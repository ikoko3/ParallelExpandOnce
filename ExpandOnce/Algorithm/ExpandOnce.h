#pragma once
#include "../Graph/Graph.h"
#include "../Graph/NodePair.h"

namespace alg {
	class ExpandOnce
	{
	public:
		explicit ExpandOnce(csr::Graph* graph1, csr::Graph* graph2, int threshold, int a,csr::MatchedPairsSet* seedSet);
		virtual csr::MatchedPairsSet* Run();
	protected:
		csr::Graph* Graph1;
		csr::Graph* Graph2;
		csr::MatchedPairsSet* SeedSet;
		int Threshold;
		int ExpandedSeedSize;
	};

	class ExpandOnceSerial : public ExpandOnce {
	public:
		using ExpandOnce::ExpandOnce;
		csr::MatchedPairsSet* Run();
	};

	class ExpandOnceParallel : public ExpandOnce {
	public:
		using ExpandOnce::ExpandOnce;
		csr::MatchedPairsSet* Run();
	};
}


