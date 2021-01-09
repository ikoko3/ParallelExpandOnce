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

	MatchedPairsSet* matchedPairs = new MatchedPairsSet();
	matchedPairs->addMatchedPairs(SeedSet);
	

	auto pairScores = new map<string, PairMatchingScore*>();

	gh::CreateNeighbouringPairs(SeedSet->getNodeSets(), Graph1, Graph2,pairScores);
	for (auto pairMapItem : *pairScores) {
		auto pairScore = pairMapItem.second;
		if (pairScore->getScore() >= Threshold
			&& !matchedPairs->GraphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
			&& !matchedPairs->GraphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
		{
			matchedPairs->addNodePair(pairScore->getPair());
		}
	}

	MatchedPairsSet* usedPairs = new MatchedPairsSet();
	usedPairs->addMatchedPairs(SeedSet);

	auto diff = usedPairs->getDifference(matchedPairs);
	srand(time(NULL));
	bool foundAtLeastOneOverThreshold = true;
	while (diff.size() > 0 && foundAtLeastOneOverThreshold) {
		auto randomPair = diff[rand() % (diff.size() - 1)];
		usedPairs->addNodePair(randomPair);

		gh::CreateNeighbouringPairs(deque<NodePair*>({randomPair}), Graph1, Graph2, pairScores);
		foundAtLeastOneOverThreshold = false;
		for (auto pairMapItem : *pairScores) {
			auto pairScore = pairMapItem.second;
			if (pairScore->getScore() >= Threshold
				&& !matchedPairs->GraphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
				&& !matchedPairs->GraphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
			{
				matchedPairs->addNodePair(pairScore->getPair());
				foundAtLeastOneOverThreshold = true;
			}
		}
		diff = usedPairs->getDifference(matchedPairs);
	}

	delete usedPairs;
	for (auto pair : *pairScores) {
		delete pair.second;
	}
	delete pairScores;

	return matchedPairs;
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
