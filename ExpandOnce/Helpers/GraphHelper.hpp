#include <string>
#include <map>
#include <deque>
#include "../Graph/Graph.h"
#include "../Graph/NodePair.h"

using namespace std;
using namespace csr;

namespace gh
{
	Graph* CreateGraphFromFile(string fileName, int skipLinesCount = 0);
	MatchedPairsSet* CreateSeedSetFromFile(string fileName);
	void CreateNeighbouringPairs(deque<NodePair*> nodePairs, Graph* g1, Graph* g2, map<string, PairMatchingScore*>* pairScores);
}