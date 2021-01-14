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




struct MyHashCompare {
	static size_t hash(const string& x) {
		size_t h = 0;
		for (const char* s = x.c_str(); *s; ++s)
			h = (h * 17) ^ *s;
		return h;
	}
	//! True if strings are equal
	static bool equal(const string& x, const string& y) {
		return x == y;
	}
};

typedef concurrent_hash_map<string, PairMatchingScore*, MyHashCompare> PairScores;


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

void voteNeighbouringEdges(int g1edge, vector<int>* g2edges,PairScores* pairScores) {

		for (auto &g2edge : *g2edges) {
			auto pair = new NodePair(g1edge, g2edge);
			auto pairKey = pair->getKey();

			PairScores::accessor acc;
			auto found = pairScores->find(acc, pairKey);
			if (!found) {
				pairScores->insert(acc, pairKey);
				acc->second = new PairMatchingScore(pair);;
			}
			else {
				acc->second->incrementScore();
			}
		}	
}



void removeUnusedNeighbouringPairs(Graph * g1, Graph * g2, PairScores* pairScores, MatchedPairsSet* M)
{
	vector<string>* keysToRemove = new vector<string>();
	auto it = pairScores->begin();

	//parallel_for(blocked_range<size_t>(0, pairScores->size()), CountUnusedPairs(it, keysToRemove, M));
	for (auto it = pairScores->begin(); it != pairScores->end(); it++)
		checkUnusedPairs(it, keysToRemove, M);
	
	parallel_for(blocked_range<size_t>(0, keysToRemove->size()), RemoveUnusedPairs(keysToRemove, pairScores));	
}


struct ApplyVotes {
	vector<int>* const _g1edges;
	vector<int>* const _g2edges;
	PairScores* const _pairScores;
public:
	void operator()(const blocked_range<size_t>& r) const {
		vector<int>* edges = _g1edges;
		for (size_t i = r.begin(); i != r.end(); ++i)
			voteNeighbouringEdges((*edges)[i],_g2edges,_pairScores);
	}
	ApplyVotes(vector<int>* g1edges, vector<int>* g2edges,PairScores* pairScores) : 
		_g1edges(g1edges),_g2edges(g2edges),_pairScores(pairScores){}
};

void ParallelVoteNeighbours(vector<int>* g1edges, size_t n,vector<int>* g2edges, PairScores* pairScores) {
	parallel_for(blocked_range<size_t>(0, n), ApplyVotes(g1edges,g2edges,pairScores));
}


void createNeighbouringPairs(NodePair * nodePair, Graph * g1, Graph * g2, PairScores* pairScores)
{
	PairScores table;

	vector<int>* g1edges = g1->getNeighboursFor(nodePair->getNodeId(graph1));
	vector<int>* g2edges = g1->getNeighboursFor(nodePair->getNodeId(graph2));

	ParallelVoteNeighbours(g1edges,g1edges->size(), g2edges,pairScores);

	delete g1edges;
	delete g2edges;
}

void createNeighbouringPairs(deque<NodePair*> nodePairs, Graph* g1, Graph* g2, PairScores* pairScores)
{
	for (auto &nodeSet : nodePairs)
		createNeighbouringPairs(nodeSet, g1, g2, pairScores);


	PairScores::iterator it = pairScores->begin();
	//cout << "_____________" << endl;
	//while ( it != pairScores->end()) {
	//	it->second->print();
	//	it++;
	//}
	//cout << "_____________" << endl;
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
		if (counter % 5 == 0) {
			removeUnusedNeighbouringPairs(Graph1, Graph2, pairScores, M);
		}
		diff = Z->getDifference(M);
	}

	delete Z;
	for (auto pair : *pairScores) {
		delete pair.second;
	}
	delete pairScores;

	return M;
}