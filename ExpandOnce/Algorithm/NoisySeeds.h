#pragma once
#include <set>
#include <deque>
#include "../Graph/Graph.h"
#include "../Graph/NodePair.h"


namespace alg {
	class NoisySeeds
	{
	public:
		explicit NoisySeeds(csr::Graph* graph1, csr::Graph* graph2, int threshold, csr::MatchedPairsSet* seedSet);
		virtual csr::MatchedPairsSet* run();
	protected:
		csr::Graph* Graph1;
		csr::Graph* Graph2;
		csr::MatchedPairsSet* SeedSet;
		int Threshold;
	};


	class NoisySeedsSerial : public NoisySeeds {
	public:
		using NoisySeeds::NoisySeeds;
		csr::MatchedPairsSet* run();
	};

	class NoisySeedsParallel : public NoisySeeds {
	public:
		using NoisySeeds::NoisySeeds;
		csr::MatchedPairsSet* run();
	};
}
