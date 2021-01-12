#include <deque>
#include <time.h> 
#include "NoisySeeds.h"
#include "../Helpers/GraphHelper.hpp"
#include "tbb/concurrent_hash_map.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

using namespace tbb;


void createNeighbouringPairs(NodePair * nodePair, Graph * g1, Graph * g2, map<string, PairMatchingScore*>* pairScores)
{
	vector<int>* g1edges = g1->getNeighboursFor(nodePair->getNodeId(graph1));
	vector<int>* g2edges = g1->getNeighboursFor(nodePair->getNodeId(graph2));

	for (auto &g1edge : *g1edges) {
		for (auto &g2edge : *g2edges) {
			auto pair = new NodePair(g1edge, g2edge);
			auto pairKey = pair->getKey();

			auto it = pairScores->find(pairKey);
			PairMatchingScore* pairScore;

			if (it == pairScores->end()) {
				pairScore = new PairMatchingScore(pair);
				(*pairScores)[pairKey] = pairScore;
			}
			else {
				pairScore = it->second;
				pairScore->incrementScore();
			}

			//pairScore->Print();
		}
	}

	delete g1edges;
	delete g2edges;
}

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

// Function object for counting occurrences of strings.
struct VoteNeighbours {
	PairScores& table;
	VoteNeighbours(PairScores& table_) : table(table_) {}
	void operator()(const blocked_range<string*> range) const {
		for (string* p = range.begin(); p != range.end(); ++p) {
			PairScores::accessor a;
			/*table.insert(a, *p);
			a->second += 1;*/
		}
	}
};


MatchedPairsSet* alg::NoisySeedsParallel::run()
{
	//cout << "Starting Parallel Noisy Seeds with threshold " << Threshold << endl;

	//MatchedPairsSet* M = new MatchedPairsSet();
	//M->addMatchedPairs(SeedSet);

	//vector<int>* g1edges = g1->getNeighboursFor(nodePair->getNodeId(graph1));
	//vector<int>* g2edges = g1->getNeighboursFor(nodePair->getNodeId(graph2));


	/*auto pairScores = new map<string, PairMatchingScore*>();

	gh::createNeighbouringPairs(SeedSet->getNodeSets(), Graph1, Graph2, pairScores);
	for (auto pairMapItem : *pairScores) {
		auto pairScore = pairMapItem.second;
		if (pairScore->getScore() >= Threshold
			&& !M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
			&& !M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
		{
			M->addNodePair(pairScore->getPair());
		}
	}

	MatchedPairsSet* Z = new MatchedPairsSet();
	Z->addMatchedPairs(SeedSet);

	gh::removeUsedNeighbouringPairs(Graph1, Graph2, pairScores, M);

	auto diff = Z->getDifference(M);
	int counter = 0;
	srand(time(NULL));
	while (diff.size() > 0) {
		auto randomPair = diff[rand() % diff.size()];
		Z->addNodePair(randomPair);

		gh::createNeighbouringPairs(randomPair, Graph1, Graph2, pairScores);

		for (auto it = pairScores->cbegin(); it != pairScores->cend();)
		{
			auto pairScore = it->second;

			if (!M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
				&& !M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
			{
				if (pairScore->getScore() >= Threshold)
					M->addNodePair(pairScore->getPair());
				++it;
			}
			else {
				delete pairScore;
				it = pairScores->erase(it);
			}
		}

		diff = Z->getDifference(M);
	}

	delete Z;
	for (auto pair : *pairScores) {
		delete pair.second;
	}
	delete pairScores;*/

	return nullptr;
}