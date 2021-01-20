#include <deque>
#include <time.h> 
#include <chrono>
#include <list>  
#include "NoisySeeds.h"
#include <algorithm>
#include "../Helpers/GraphHelper.hpp"
#include "tbb/concurrent_hash_map.h"
#include "tbb/concurrent_vector.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

using namespace tbb;
using namespace std;
using namespace gh;





void checkUnusedPairs(PairScores::iterator it, concurrent_vector<string>* keysToRemove, MatchedPairsSet* M) {
	auto pairScore = it->second;
	if (M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
		|| M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2))) {
		keysToRemove->push_back(pairScore->getPairKey());
	}
	it++;
}

void checkPairsForThreshold(PairScores::iterator it, MatchedPairsSet* M, concurrent_vector<string>* keysToRemove, int threshold, concurrent_vector<PairMatchingScore*>* newPairs) {
	auto pairScore = it->second;


	if (!M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
		&& !M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
	{
		if (pairScore->getScore() >= threshold) {
			newPairs->push_back(pairScore);
		}
	}
	else {
		keysToRemove->push_back(it->second->getPairKey());
	}
}

void checkPairsForThresholdOLD(PairScores::iterator it, MatchedPairsSet* M, concurrent_vector<string>* keysToRemove, int threshold) {
	auto pairScore = it->second;

	if (!M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
		&& !M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
	{
		if (pairScore->getScore() >= threshold)
			M->addNodePair(pairScore->getPair());
	}
	else {
		keysToRemove->push_back(it->second->getPairKey());
	}
}


void removeKey(string key, PairScores* pairScores) {
	//cout << key << endl;
	PairScores::accessor acc;
	auto found = pairScores->find(acc, key);
	
	if (found) {
		delete acc->second;
		pairScores->erase(acc);
	}
		
}

struct CheckPairsThreshold {
	PairScores* const _pairScores;
	concurrent_vector<string>* const _keys;
	MatchedPairsSet* const _M;
	int const _threshold;
	concurrent_vector<PairMatchingScore*>* const _newPairs;
public:
	void operator()(const PairScores::range_type& r) const {
		for (PairScores::iterator i = r.begin(); i != r.end(); ++i) 
			checkPairsForThreshold(i, _M, _keys, _threshold,_newPairs);
	}
	CheckPairsThreshold(concurrent_vector<string>* keys, PairScores* pairScores, MatchedPairsSet* M, int threshold, concurrent_vector<PairMatchingScore*>* newPairs)
		: _keys(keys), _pairScores(pairScores), _M(M), _threshold(threshold), _newPairs(newPairs) {}
};

struct RemoveUnusedPairs {
	concurrent_vector<string>* const _keys;
	PairScores* const _pairScores;
public:
	void operator()(const blocked_range<size_t>& r) const {
		for (size_t i = r.begin(); i != r.end(); ++i)
			removeKey((*_keys)[i], _pairScores);
	}
	RemoveUnusedPairs(concurrent_vector<string>* keys, PairScores* pairScores) : _keys(keys), _pairScores(pairScores) {}
};


void removeUnusedNeighbouringPairs(Graph * g1, Graph * g2, PairScores* pairScores, MatchedPairsSet* M)
{
	concurrent_vector<string>* keysToRemove = new concurrent_vector<string>();
	auto it = pairScores->begin();

	//parallel_for(blocked_range<size_t>(0, pairScores->size()), CountUnusedPairs(it, keysToRemove, M));
	for (auto it = pairScores->begin(); it != pairScores->end(); it++)
		checkUnusedPairs(it, keysToRemove, M);
	
	parallel_for(blocked_range<size_t>(0, keysToRemove->size()), RemoveUnusedPairs(keysToRemove, pairScores));	
}

bool compareScores(PairMatchingScore* s1, PairMatchingScore* s2)
{
	if (s1->getScore() != s2->getScore()) {
		cout << "Found Pair with different score " << s1->getScore() << ":" <<s2->getScore() << endl;
	}

	return (s1->getScore()  < s2->getScore());
}



MatchedPairsSet* alg::NoisySeedsParallel::run()
{
	int graphSize = Graph1->getNodesCount();
	cout << "Starting Parallel Noisy Seeds with threshold " << Threshold
		<< " (" << graphSize << ") Nodes (G1:" << Graph1->getEdgesCount() << "/G2:" << Graph2->getEdgesCount() << ") edges" << endl;

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

	concurrent_vector<string>* keysToRemove;
	concurrent_vector<PairMatchingScore*>* newPairs;
	while (diff.size() > 0) {
		auto randomPair = diff[rand() % diff.size()];
		Z->addNodePair(randomPair);

		createNeighbouringPairs(randomPair, Graph1, Graph2, pairScores);
		keysToRemove = new concurrent_vector<string>();
		newPairs = new concurrent_vector<PairMatchingScore*>();

		//cout << "Pair Scores size:" << pairScores->size() << endl;
		parallel_for(pairScores->range(), 
			CheckPairsThreshold(keysToRemove,pairScores,M,Threshold, newPairs));



		//check new pairs
		set<int> g1Used;
		set<int> g2Used;
		//sort(newPairs->begin(), newPairs->end(),compareScores);
		//cout << "-----" << endl;
		for (auto it = newPairs->begin(); it != newPairs->end(); ++it) {
			auto pair = it[0]->getPair();
			//it[0]->print();
			if (!g1Used.count(pair->getNodeId(graph1)) > 0
				&& !g2Used.count(pair->getNodeId(graph2)) > 0)
			{
				M->addNodePair(pair);
				g1Used.insert(pair->getNodeId(graph1));
				g2Used.insert(pair->getNodeId(graph2));
			}
			else {
				keysToRemove->push_back(pair->getKey());
			}
		}
			


		delete newPairs;


	/*	for (auto it = pairScores->begin(); it != pairScores->end();it++)
		{
			checkPairsForThreshold(it, M, keysToRemove, Threshold);
		}*/

		//Reduce
		parallel_for(blocked_range<size_t>(0, keysToRemove->size()), RemoveUnusedPairs(keysToRemove, pairScores));
		delete keysToRemove;

		if (counter++ % 100 == 0) {
			int pairedCount = M->getNodeSets().size();
			cout << (float) pairedCount/ (float)graphSize * 100 << "% (" << pairedCount <<")" << endl;
		}
		if (M->getNodeSets().size() == graphSize)
			break;
			
		diff = Z->getDifference(M);
	}

	delete Z;
	for (auto pair : *pairScores) {
		delete pair.second;
	}
	delete pairScores;

	return M;
}