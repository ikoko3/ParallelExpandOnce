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

	auto pairScores = gh::CreateNeighbouringPairs(SeedSet, Graph1, Graph2);
	for (auto pairMapItem : pairScores) {
		auto pairScore = pairMapItem.second;
		if (pairScore->getScore() >= Threshold
			&& !SeedSet->GraphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
			&& !SeedSet->GraphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2))) 
		{
			matchedPairs->addNodePair(pairScore->getPair());
		}
		else 
			delete pairScore;
	}

	usedPairs->AddMatchedPairs(SeedSet);

	auto diff = usedPairs->GetDifference(matchedPairs);
	srand(time(NULL));
	while (diff.size() > 0) {
		auto randomPair = diff[rand() % (diff.size() - 1)];
		usedPairs->addNodePair(randomPair);

		//Vote neighbour nodes
		//Check threshold
		//add to the matched set

		//TODO:remove break after implementation
		break;
		diff = usedPairs->GetDifference(matchedPairs);
	}




	usedPairs->print();
	delete usedPairs;
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
