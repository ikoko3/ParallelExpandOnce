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

	auto A0i = new MatchedPairsSet();
	A0i->addMatchedPairs(SeedSet);
	auto A = new MatchedPairsSet();
	A->addMatchedPairs(SeedSet);

	while (A0i->getNodeSets().size() < ExpandedSeedSize) {
		auto Z = new MatchedPairsSet();
		auto U = new MatchedPairsSet();
		U->addMatchedPairs(A0i);
		for (auto pair : A->getNodeSets()) {
			auto pairScores = new map<string, PairMatchingScore*>();
			gh::CreateNeighbouringPairs(pair, Graph1, Graph2, pairScores);
			for (auto pairMapItem : *pairScores) {
				auto pairScore = pairMapItem.second;
				if (A0i->getNodeSets().size() < ExpandedSeedSize 
					&& !U->GraphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
					&& !U->GraphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
				{
					Z->addNodePair(pairScore->getPair());
					A0i->addNodePair(pairScore->getPair());
				}

			}
		}

		delete A;
		delete U;
		A = Z;
	}
	//A0i->print();

	NoisySeedsSerial noisySeeds(Graph1, Graph2, Threshold, A0i);
	auto matchedValues = noisySeeds.run();

	return matchedValues;
}

csr::MatchedPairsSet * alg::ExpandOnceParallel::run()
{
	//Not implemented yet
	return nullptr;
}
