#include <deque>
#include <time.h> 
#include <chrono>
#include "NoisySeeds.h"
#include "../Helpers/GraphHelper.hpp"
#include "tbb/concurrent_hash_map.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

using namespace tbb;


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

void voteNeighbouringEdges(int g1edge, vector<int>* g2edges,PairScores* pairScores) {

		for (auto &g2edge : *g2edges) {
			auto pair = new NodePair(g1edge, g2edge);
			auto pairKey = pair->getKey();

			PairScores::accessor acc;
			auto found = pairScores->find(acc, pairKey);
			if (!found) {
				pairScores->insert(acc, pairKey);
				acc->second = new PairMatchingScore(pair);;
				//cout << "Key " << pairKey << " not found" << endl;
			}
			else {
				acc->second->incrementScore();
			}
		}	
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
	//while ( it != pairScores->end()) {
	//	it->second->print();
	//	it++;
	//}
}



MatchedPairsSet* alg::NoisySeedsParallel::run()
{

	cout << "Starting Parallel Noisy Seeds with threshold " << Threshold << endl;

	MatchedPairsSet* M = new MatchedPairsSet();
	M->addMatchedPairs(SeedSet);

	std::chrono::steady_clock::time_point beginp = std::chrono::steady_clock::now();
	auto pairScores = new PairScores();
	createNeighbouringPairs(SeedSet->getNodeSets(), Graph1, Graph2, pairScores);
	std::chrono::steady_clock::time_point endp = std::chrono::steady_clock::now();

	std::chrono::steady_clock::time_point begins = std::chrono::steady_clock::now();
	auto pairScores1 = new map<string, PairMatchingScore*>();
	gh::createNeighbouringPairs(SeedSet->getNodeSets(), Graph1, Graph2, pairScores1);
	std::chrono::steady_clock::time_point ends = std::chrono::steady_clock::now();


	std::cout << "Total time PARALLEL: " << std::chrono::duration_cast<std::chrono::milliseconds> (endp - beginp).count() << "[ms]" << std::endl;
	std::cout << "Total time SERIAL: " << std::chrono::duration_cast<std::chrono::milliseconds> (ends - begins).count() << "[ms]" << std::endl;


	cout << "--------------SERIAL PAIRS-------------" << endl;


	/*map<string, PairMatchingScore*>::iterator it = pairScores1->begin();
	while (it != pairScores1->end()) {
		it->second->print();
		it++;
	}*/

	/*for (auto pairMapItem : *pairScores) {
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