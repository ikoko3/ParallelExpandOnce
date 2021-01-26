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

	MatchedPairsSet* Z;
	MatchedPairsSet* U;

	auto pairScores = new PairScores();
	while (A0i->getNodeSets().size() < ExpandedSeedSize) {
		Z = new MatchedPairsSet();
		U = new MatchedPairsSet();
		U->addMatchedPairs(A0i);
		for (auto pair : A->getNodeSets()) {
			
			gh::createNeighbouringPairs(pair, Graph1, Graph2, pairScores);
			for (auto it = pairScores->begin(); it != pairScores->end();) {
				auto pairScore = it->second;
				if (A0i->getNodeSets().size() < ExpandedSeedSize
					&& !A0i->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
					&& !A0i->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
				{
					Z->addNodePair(pairScore->getPair());
					A0i->addNodePair(pairScore->getPair());

					++it;
				}
				else {
					PairScores::accessor acc;
					auto found = pairScores->find(acc, it->second->getPairKey());
					
					if (found) {
						delete pairScore;
						pairScores->erase(acc);
					}
					++it;
				}

			}
		}

		delete A;
		delete U;
		A = Z;
	}
	delete Z;
	A0i->print();

	NoisySeedsParallel noisySeeds(Graph1, Graph2, Threshold, A0i);
	//auto matchedValues = noisySeeds.run();

	return A0i;
}