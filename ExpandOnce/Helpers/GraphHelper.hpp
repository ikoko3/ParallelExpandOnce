#include <string>
#include <map>
#include <deque>
#include "../Graph/Graph.h"
#include "../Graph/NodePair.h"
#include "tbb/concurrent_hash_map.h"

using namespace std;
using namespace csr;

namespace gh
{
	struct MyHashCompare {
		static size_t hash(const string& x) {
			size_t h = 0;
			for (const char* s = x.c_str(); *s; ++s)
				h = (h * 17) ^ *s;
			return h;
		}
		//! True if strings are equal
		static bool equal(const string& x, const string& y) {
			return x == y;
		}
	};

	typedef tbb::concurrent_hash_map<string, PairMatchingScore*, MyHashCompare> PairScores;

	Graph* createGraphFromFile(string fileName, int skipLinesCount = 0);
	MatchedPairsSet* createSeedSetFromFile(string fileName);

	void createNeighbouringPairs(deque<NodePair*> nodePairs, Graph* g1, Graph* g2, map<string, PairMatchingScore*>* pairScores);
	void createNeighbouringPairs(NodePair* nodePair, Graph* g1, Graph* g2, map<string, PairMatchingScore*>* pairScores);
	
	void createNeighbouringPairs(NodePair * nodePair, Graph * g1, Graph * g2, PairScores* pairScores);
	void createNeighbouringPairs(deque<NodePair*> nodePairs, Graph* g1, Graph* g2, PairScores* pairScores);


	void removeUnusedNeighbouringPairs(Graph* g1, Graph* g2, map<string, PairMatchingScore*>* pairScores, MatchedPairsSet* M);
}