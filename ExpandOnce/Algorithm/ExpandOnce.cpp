#include "ExpandOnce.h"
#include "NoisySeeds.h"
#include "../Helpers/GraphHelper.hpp"
#include <map>


using namespace alg;
using namespace csr;
using namespace std;

ExpandOnce::ExpandOnce(csr::Graph * graph1, csr::Graph * graph2, int threshold,int expandedSeedSize, MatchedPairsSet * seedSet)
{
	Graph1 = graph1;
	Graph2 = graph2;
	SeedSet = seedSet;
	Threshold = threshold;
	this->ExpandedSeedSize = expandedSeedSize;
}

csr::MatchedPairsSet * alg::ExpandOnce::run()
{
	//asbtract
	throw;
}

csr::MatchedPairsSet * alg::ExpandOnceSerial::run()
{
	cout << "Started expand once"<< endl;

	auto expandedSeedSet = new MatchedPairsSet();
	expandedSeedSet->addMatchedPairs(SeedSet);
	auto A = new MatchedPairsSet();
	A->addMatchedPairs(SeedSet);
	while (expandedSeedSet->getNodeSets().size() < ExpandedSeedSize) {
		auto Z = new MatchedPairsSet();
		auto U = new MatchedPairsSet();
		U->addMatchedPairs(expandedSeedSet);
		for (auto pair : A->getNodeSets()) {
			auto pairScores = new map<string, PairMatchingScore*>();
			gh::CreateNeighbouringPairs(deque<NodePair*>({ pair }), Graph1, Graph2, pairScores);
			for (auto pairMapItem : *pairScores) {
				auto pairScore = pairMapItem.second;
				if (expandedSeedSet->getNodeSets().size() < ExpandedSeedSize 
					&& !U->GraphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
					&& !U->GraphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
				{
					Z->addNodePair(pairScore->getPair());
					expandedSeedSet->addNodePair(pairScore->getPair());
				}

			}
		}

		delete A;
		delete U;
		A = Z;
	}


	NoisySeedsSerial noisySeeds(Graph1, Graph2, Threshold, expandedSeedSet);
	auto matchedValues = noisySeeds.run();

	return matchedValues;
}

csr::MatchedPairsSet * alg::ExpandOnceParallel::run()
{
	//Not implemented yet
	return nullptr;
}
