#include <deque>
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
	MatchedPairsSet* matchedPairs = new MatchedPairsSet();
	cout << "Starting Serial Noisy Seeds with threshold " << Threshold << endl;

	auto pairScores = gh::CreateNeighbouringPairs(SeedSet, Graph1, Graph2);
	for (auto pairMapItem : pairScores) {
		auto pairScore = pairMapItem.second;

		if (pairScore->getScore() >= Threshold
			&& !SeedSet->GraphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
			&& !SeedSet->GraphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
		{
			matchedPairs->addNodePair(pairScore->getPair());
		}

	}

	//TODO:Implement lines 5-10 from paper

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
