#pragma once
#include <set>
#include <deque>
#include "../Graph/Graph.h"
#include "../Graph/SeedSet.h"


namespace alg {
	class NoisySeeds
	{
	public:
		explicit NoisySeeds(csr::Graph* graph1, csr::Graph* graph2, int threshold, csr::SeedSet* seedSet);
		void virtual Run();
	protected:
		csr::Graph* Graph1;
		csr::Graph* Graph2;
		csr::SeedSet* SeedSet;
		int Threshold;
	};


	class NoisySeedsSerial : public NoisySeeds {
	public:
		using NoisySeeds::NoisySeeds;
		void Run();
	};

	class NoisySeedsParallel : public NoisySeeds {
	public:
		using NoisySeeds::NoisySeeds;
		void Run();
	};
}
