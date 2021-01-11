#include <deque>
#include <time.h> 
#include "NoisySeeds.h"
#include "../Helpers/GraphHelper.hpp"


using namespace alg;
using namespace csr;
using namespace std;

NoisySeeds::NoisySeeds(csr::Graph * graph1, csr::Graph * graph2, int threshold, csr::MatchedPairsSet * seedSet)
{
	Graph1 = graph1;
	Graph2 = graph2;
	SeedSet = seedSet;
	Threshold = threshold;
}

MatchedPairsSet* NoisySeedsSerial::run()
{
	cout << "Starting Serial Noisy Seeds with threshold " << Threshold << endl;

	MatchedPairsSet* M = new MatchedPairsSet();
	M->addMatchedPairs(SeedSet);
	

	auto pairScores = new map<string, PairMatchingScore*>();

	gh::createNeighbouringPairs(SeedSet->getNodeSets(), Graph1, Graph2,pairScores);
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

    gh::removeUsedNeighbouringPairs(Graph1,Graph2,pairScores,M);

	auto diff = Z->getDifference(M);
	int counter = 0;
	srand(time(NULL));
	while (diff.size() > 0) {
		auto randomPair = diff[rand() % diff.size()];
		Z->addNodePair(randomPair);

		gh::createNeighbouringPairs(randomPair, Graph1, Graph2, pairScores);

		for (auto pairMapItem : *pairScores) {
			auto pairScore = pairMapItem.second;
			if (pairScore->getScore() >= Threshold
				&& !M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
				&& !M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
			{
				M->addNodePair(pairScore->getPair());
			}
		}
		diff = Z->getDifference(M);

		if (counter % 25 == 0) {
			gh::removeUsedNeighbouringPairs(Graph1, Graph2, pairScores, M);
		}
			
		counter++;
	}

	delete Z;
	for (auto pair : *pairScores) {
		delete pair.second;
	}
	delete pairScores;

	return M;
}

MatchedPairsSet* alg::NoisySeedsParallel::run()
{
	//not implemented yet
	throw;
}



MatchedPairsSet* alg::NoisySeeds::run()
{
	//only for overide
	throw;
}
