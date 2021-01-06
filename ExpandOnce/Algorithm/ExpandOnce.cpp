#include "ExpandOnce.h"


using namespace alg;
using namespace csr;
using namespace std;

ExpandOnce::ExpandOnce(csr::Graph * graph1, csr::Graph * graph2, int threshold,int a, csr::MatchedPairsSet * seedSet)
{
	Graph1 = graph1;
	Graph2 = graph2;
	SeedSet = seedSet;
	Threshold = threshold;
	this->a = a;
}

csr::MatchedPairsSet * alg::ExpandOnce::Run()
{
	//asbtract
	throw;
}

csr::MatchedPairsSet * alg::ExpandOnceSerial::Run()
{
	//Not Implemented yet
	return nullptr;
}

csr::MatchedPairsSet * alg::ExpandOnceParallel::Run()
{
	//Not implemented yet
	return nullptr;
}
