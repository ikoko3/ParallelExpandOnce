#include <string>
#include <map>
#include <deque>
#include "../Graph/Graph.h"
#include "../Graph/NodePair.h"

using namespace std;
using namespace csr;

namespace gh
{
	Graph* createGraphFromFile(string fileName, int skipLinesCount = 0);
	MatchedPairsSet* createSeedSetFromFile(string fileName);
	void createNeighbouringPairs(deque<NodePair*> nodePairs, Graph* g1, Graph* g2, map<string, PairMatchingScore*>* pairScores);
	void createNeighbouringPairs(NodePair* nodePair, Graph* g1, Graph* g2, map<string, PairMatchingScore*>* pairScores);
	void removeUsedNeighbouringPairs(Graph* g1, Graph* g2, map<string, PairMatchingScore*>* pairScores, MatchedPairsSet* M);
}