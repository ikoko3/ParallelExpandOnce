#include "ExpandOnce.h"
#include "NoisySeeds.h"
#include "../Helpers/GraphHelper.hpp"

using namespace gh;

csr::MatchedPairsSet * alg::ExpandOnceParallel::run()
{
	cout << "Started expand once" << endl;

	auto A0i = new MatchedPairsSet();
	A0i->addMatchedPairs(SeedSet);
	auto A = new MatchedPairsSet();
	A->addMatchedPairs(SeedSet);

	while (A0i->getNodeSets().size() < ExpandedSeedSize) {
		auto Z = new MatchedPairsSet();
		auto U = new MatchedPairsSet();
		U->addMatchedPairs(A0i);
		for (auto pair : A->getNodeSets()) {
			auto pairScores = new PairScores();
			gh::createNeighbouringPairs(pair, Graph1, Graph2, pairScores);
			for (auto pairMapItem : *pairScores) {
				auto pairScore = pairMapItem.second;
				if (A0i->getNodeSets().size() < ExpandedSeedSize
					&& !U->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
					&& !U->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
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

	NoisySeedsParallel noisySeeds(Graph1, Graph2, Threshold, A0i);
	auto matchedValues = noisySeeds.run();

	return matchedValues;
}