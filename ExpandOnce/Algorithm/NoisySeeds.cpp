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
	int graphSize = Graph1->getNodesCount();
	cout << "Starting Serial Noisy Seeds with threshold " << Threshold
		<< " ("<< graphSize << ") Nodes (G1:"<<Graph1->getEdgesCount() << "/G2:" << Graph2->getEdgesCount()<<") edges" <<endl;

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
	//M->print();

	MatchedPairsSet* Z = new MatchedPairsSet();
	Z->addMatchedPairs(SeedSet);

    gh::removeUnusedNeighbouringPairs(Graph1,Graph2,pairScores,M);

	auto diff = Z->getDifference(M);
	int counter = 0;
	srand(time(NULL));
	while (diff->size() > 0) {
		auto randomPair = (*diff)[rand() % diff->size()];
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
		if (counter++ % 100 == 0) {
			int pairedCount = M->getNodeSets().size();
			cout << (float)pairedCount / (float)graphSize * 100 << "% (" << pairedCount << ")" << endl;
		}
		if (M->getNodeSets().size() == graphSize)
			break;

		delete diff;
		diff = Z->getDifference(M);
	}

	delete Z;
	for (auto pair : *pairScores) {
		delete pair.second;
	}
	delete pairScores;

	return M;
}





MatchedPairsSet* alg::NoisySeeds::run()
{
	//abstract
	throw;
}
