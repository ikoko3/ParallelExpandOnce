#include <string>
#include <map>
#include "../Graph/Graph.h"
#include "../Graph/SeedSet.h"
#include "../Graph/Pair.h"

using namespace std;
using namespace csr;

namespace gh
{
	Graph* CreateGraphFromFile(string fileName, int skipLinesCount = 0);
	SeedSet* CreateSeedSetFromFile(string fileName);
	map<string, PairMatchingScore*> CreateNeighbouringPairs(SeedSet* Set,Graph* g1,Graph* g2);
}