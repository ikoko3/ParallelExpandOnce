#include <string>
#include <map>
#include <deque>
#include "../Graph/Graph.h"
#include "../Graph/NodePair.h"
#include "tbb/concurrent_hash_map.h"
#include "tbb/concurrent_vector.h"


using namespace std;
using namespace csr;
using namespace tbb;

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

	void removeKey(string key, PairScores* pairScores);

	struct RemoveUnusedPairs {
		concurrent_vector<string>* const _keys;
		PairScores* const _pairScores;
	public:
		void operator()(const blocked_range<size_t>& r) const {
			for (size_t i = r.begin(); i != r.end(); ++i)
				removeKey((*_keys)[i], _pairScores);
		}
		RemoveUnusedPairs(concurrent_vector<string>* keys, PairScores* pairScores) : _keys(keys), _pairScores(pairScores) {}
	};

	struct MatchedPairsResults {
		int threshold;
		int seedSetSize;
		int nodes;
		int g1Edges;
		int g2Edges;
		int nIdent;
		SetAccuraccy* serialAccuracy;
		SetAccuraccy* parallelAccuracy;
		long serialTime;
		long parallelTime;

		MatchedPairsResults(int _threshold,int _seedSetSize, int _nodes, int _g1Edges, int _g2Edges, int _nIdent
		,long _serialTime, long _parallelTime, SetAccuraccy* _serialAccuracy, SetAccuraccy* _parallelAccuracy) 
		{
			threshold = _threshold;
			seedSetSize = _seedSetSize;
			nodes = _nodes;
			g1Edges = _g1Edges;
			g2Edges = _g2Edges;
			nIdent = _nIdent;
			serialAccuracy = _serialAccuracy;
			parallelAccuracy = _parallelAccuracy;
			serialTime = _serialTime;
			parallelTime = _parallelTime;
		}

		~MatchedPairsResults() {
				delete serialAccuracy;
				delete parallelAccuracy;
		}
	};

	void saveMatchedResultsToFile(MatchedPairsResults* results);

	int getNIdent(Graph* g1, Graph* g2);

}