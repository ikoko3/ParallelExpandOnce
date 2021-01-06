#include <deque>
#include <time.h> 
#include "NoisySeeds.h"
#include "../Helpers/GraphHelper.hpp"


using namespace alg;
using namespace csr;
using namespace std;

alg::NoisySeeds::NoisySeeds(csr::Graph * graph1, csr::Graph * graph2, int threshold, csr::MatchedPairsSet * seedSet)
{
	Graph1 = graph1;
	Graph2 = graph2;
	SeedSet = seedSet;
	Threshold = threshold;
}

MatchedPairsSet* NoisySeedsSerial::Run()
{
	cout << "Starting Serial Noisy Seeds with threshold " << Threshold << endl;

	MatchedPairsSet* matchedPairs = new MatchedPairsSet();
	matchedPairs->AddMatchedPairs(SeedSet);
	MatchedPairsSet* usedPairs = new MatchedPairsSet();

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

	usedPairs->AddMatchedPairs(SeedSet);

	auto diff = usedPairs->GetDifference(matchedPairs);
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
		diff = usedPairs->GetDifference(matchedPairs);
	}

	delete usedPairs;
	for (auto pair : *pairScores) {
		delete pair.second;
	}
	delete pairScores;
	matchedPairs->print();

	return matchedPairs;
}

MatchedPairsSet* alg::NoisySeedsParallel::Run()
{
	//not implemented yet
	throw;
}



MatchedPairsSet* alg::NoisySeeds::Run()
{
	//only for overide
	throw;
}
