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

csr::MatchedPairsSet * alg::ExpandOnceOriginal::run()
{
	int threshold = Threshold;
	cout << "Started expand once"<< endl;

	auto A0i = new MatchedPairsSet();
	A0i->addMatchedPairs(SeedSet);
	auto A = new MatchedPairsSet();
	A->addMatchedPairs(SeedSet);
	MatchedPairsSet * Z;
	MatchedPairsSet * U;

	auto pairScores = new map<string, PairMatchingScore*>();
	while (A0i->getNodeSets().size() < ExpandedSeedSize) {
		Z = new MatchedPairsSet();
		U = new MatchedPairsSet();
		U->addMatchedPairs(A0i);
		 
		int newPairs = 0;
		for (auto pair : A->getNodeSets()) 
		{
			
			gh::createNeighbouringPairs(pair, Graph1, Graph2, pairScores);
			for (auto it = pairScores->cbegin(); it != pairScores->cend();) {
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
					delete pairScore;
					it = pairScores->erase(it);
				}

			}
			
		}

		delete A;
		delete U;
		A = Z;

	}

	for (auto pair : *pairScores) {
		delete pair.second;
	}
	delete pairScores;

	delete Z;
	A0i->print();

	NoisySeedsSerial noisySeeds(Graph1, Graph2, Threshold, A0i);
	auto matchedValues = noisySeeds.run();

	return A0i;
}

csr::MatchedPairsSet * alg::ExpandOnceSerial::run()
{
	int threshold = Threshold;
	cout << "Started expand once" << endl;

	auto A0i = new MatchedPairsSet();
	A0i->addMatchedPairs(SeedSet);
	auto A = new MatchedPairsSet();
	A->addMatchedPairs(SeedSet);
	MatchedPairsSet * Z;
	MatchedPairsSet * U;

	auto pairScores = new map<string, PairMatchingScore*>();
	while (A0i->getNodeSets().size() < ExpandedSeedSize) {
		Z = new MatchedPairsSet();
		U = new MatchedPairsSet();
		U->addMatchedPairs(A0i);

		int newPairs = 0;
		for (auto pair : A->getNodeSets())
		{

			gh::createNeighbouringPairs(pair, Graph1, Graph2, pairScores);
			for (auto it = pairScores->cbegin(); it != pairScores->cend();) {
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
					delete pairScore;
					it = pairScores->erase(it);
				}

			}

		}

		delete A;
		delete U;
		A = Z;

	}

	for (auto pair : *pairScores) {
		delete pair.second;
	}
	delete pairScores;

	delete Z;
	A0i->print();

	NoisySeedsSerial noisySeeds(Graph1, Graph2, Threshold, A0i);
	auto matchedValues = noisySeeds.run();

	return A0i;
}





