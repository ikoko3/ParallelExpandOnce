#include <deque>
#include <time.h> 
#include <chrono>
#include <list>  
#include "NoisySeeds.h"
#include "../Helpers/GraphHelper.hpp"
#include "tbb/concurrent_hash_map.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

using namespace tbb;
using namespace std;
using namespace gh;





void checkUnusedPairs(PairScores::iterator it, vector<string>* keysToRemove, MatchedPairsSet* M) {
	auto pairScore = it->second;
	if (M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
		|| M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2))) {
		keysToRemove->push_back(pairScore->getPair()->getKey());
	}
	it++;
}

void removeKey(string key, PairScores* pairScores) {
	//cout << key << endl;
	PairScores::accessor acc;
	auto found = pairScores->find(acc, key);
	if(found)
		pairScores->erase(acc);
}

struct RemoveUnusedPairs {
	vector<string>* const _keys;
	PairScores* const _pairScores;
public:
	void operator()(const blocked_range<size_t>& r) const {
		for (size_t i = r.begin(); i != r.end(); ++i)
			removeKey((*_keys)[i], _pairScores);
	}
	RemoveUnusedPairs(vector<string>* keys, PairScores* pairScores) : _keys(keys), _pairScores(pairScores) {}
};


struct CountUnusedPairs {
	PairScores::iterator const _it;
	vector<string>* const _keys;
	MatchedPairsSet* const _M;
public:
	void operator()(const blocked_range<size_t>& r) const {
		for (size_t i = r.begin(); i != r.end(); ++i)
		{
			checkUnusedPairs(_it, _keys, _M);
		}
			
	}
	CountUnusedPairs(PairScores::iterator it, vector<string>* keysToRemove, MatchedPairsSet* M) :
		_it(it), _keys(keysToRemove), _M(M) {}
};





void removeUnusedNeighbouringPairs(Graph * g1, Graph * g2, PairScores* pairScores, MatchedPairsSet* M)
{
	vector<string>* keysToRemove = new vector<string>();
	auto it = pairScores->begin();

	//parallel_for(blocked_range<size_t>(0, pairScores->size()), CountUnusedPairs(it, keysToRemove, M));
	for (auto it = pairScores->begin(); it != pairScores->end(); it++)
		checkUnusedPairs(it, keysToRemove, M);
	
	parallel_for(blocked_range<size_t>(0, keysToRemove->size()), RemoveUnusedPairs(keysToRemove, pairScores));	
}





MatchedPairsSet* alg::NoisySeedsParallel::run()
{

	cout << "Starting Parallel Noisy Seeds with threshold " << Threshold << endl;

	MatchedPairsSet* M = new MatchedPairsSet();
	M->addMatchedPairs(SeedSet);

	auto pairScores = new PairScores();
	createNeighbouringPairs(SeedSet->getNodeSets(), Graph1, Graph2, pairScores);
	for (auto pairMapItem : *pairScores) {
		auto pairScore = pairMapItem.second;
		if (pairScore->getScore() >= Threshold
			&& !M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
			&& !M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
		{
			M->addNodePair(pairScore->getPair());
		}
	}
	//M->print();

	MatchedPairsSet* Z = new MatchedPairsSet();
	Z->addMatchedPairs(SeedSet);

	removeUnusedNeighbouringPairs(Graph1, Graph2, pairScores, M);

	auto diff = Z->getDifference(M);
	int counter = 0;
	srand(time(NULL));
	while (diff.size() > 0) {
		auto randomPair = diff[rand() % diff.size()];
		Z->addNodePair(randomPair);

		createNeighbouringPairs(randomPair, Graph1, Graph2, pairScores);

		for (auto it = pairScores->begin(); it != pairScores->end();)
		{
			//PairScores::const_accessor score;
			auto pairScore = it->second;

			if (!M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
				&& !M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
			{
				if (pairScore->getScore() >= Threshold)
					M->addNodePair(pairScore->getPair());
				++it;
			}
			else {
			/*	auto key = pairScore->getPair()->getKey();
				delete pairScore;
				pairScores->erase(key);*/
				++it;
			}
			
		}
		removeUnusedNeighbouringPairs(Graph1, Graph2, pairScores, M);

		//if (counter % 15 == 0) {
		//	removeUnusedNeighbouringPairs(Graph1, Graph2, pairScores, M);
		//}
		diff = Z->getDifference(M);
	}

	delete Z;
	for (auto pair : *pairScores) {
		delete pair.second;
	}
	delete pairScores;

	return M;
}