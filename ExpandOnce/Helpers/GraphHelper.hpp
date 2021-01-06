#include <string>
#include <map>
#include "../Graph/Graph.h"
#include "../Graph/NodePair.h"

using namespace std;
using namespace csr;

namespace gh
{
	Graph* CreateGraphFromFile(string fileName, int skipLinesCount = 0);
	MatchedPairsSet* CreateSeedSetFromFile(string fileName);
	map<string, PairMatchingScore*> CreateNeighbouringPairs(MatchedPairsSet* Set,Graph* g1,Graph* g2);
}