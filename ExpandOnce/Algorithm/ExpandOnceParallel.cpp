#include <deque>
#include <chrono>
#include <list>  
#include <algorithm>

#include "ExpandOnce.h"
#include "NoisySeeds.h"
#include "../Helpers/GraphHelper.hpp"

#include "tbb/concurrent_hash_map.h"
#include "tbb/concurrent_vector.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"


using namespace tbb;
using namespace std;
using namespace gh;

void getPossiblePairs(PairScores::iterator it, MatchedPairsSet* M, concurrent_vector<string>* keysToRemove, concurrent_vector<PairMatchingScore*>* newPairs) {
	auto pairScore = it->second;

	if (!M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
		&& !M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2)))
			newPairs->push_back(pairScore);
	else 
		keysToRemove->push_back(it->second->getPairKey());
}



struct GetPossiblePairs {
	PairScores* const _pairScores;
	concurrent_vector<string>* const _keys;
	MatchedPairsSet* const _M;
	concurrent_vector<PairMatchingScore*>* const _newPairs;
public:
	void operator()(const PairScores::range_type& r) const {
		for (PairScores::iterator i = r.begin(); i != r.end(); ++i)
			getPossiblePairs(i, _M, _keys, _newPairs);
	}
	GetPossiblePairs(concurrent_vector<string>* keys, PairScores* pairScores, MatchedPairsSet* M, concurrent_vector<PairMatchingScore*>* newPairs)
		: _keys(keys), _pairScores(pairScores), _M(M), _newPairs(newPairs) {}
};


csr::MatchedPairsSet * alg::ExpandOnceParallel::run(int seed)
{
	cout << "Started expand once" << endl;

	auto A0i = new MatchedPairsSet();
	A0i->addMatchedPairs(SeedSet);
	auto A = new MatchedPairsSet();
	A->addMatchedPairs(SeedSet);

	MatchedPairsSet* Z;
	MatchedPairsSet* U;

	concurrent_vector<string>* keysToRemove;
	concurrent_vector<PairMatchingScore*>* newPairs;



	auto pairScores = new PairScores();
	while (A0i->getNodeSets().size() < ExpandedSeedSize) {
		Z = new MatchedPairsSet();
		U = new MatchedPairsSet();
		U->addMatchedPairs(A0i);
		for (auto pair : A->getNodeSets()) {

			keysToRemove = new concurrent_vector<string>();
			newPairs = new concurrent_vector<PairMatchingScore*>();

			createNeighbouringPairs(pair, Graph1, Graph2, pairScores);

			parallel_for(pairScores->range(),
				GetPossiblePairs(keysToRemove, pairScores, U, newPairs));

			set<int> g1Used;
			set<int> g2Used;
			sort(newPairs->begin(), newPairs->end(),compareScores);

			for (auto it = newPairs->begin(); it != newPairs->end(); ++it) {
				auto pair = it[0]->getPair();
				if (!g1Used.count(pair->getNodeId(graph1)) > 0
					&& !g2Used.count(pair->getNodeId(graph2)) > 0
					&& A0i->getNodeSets().size() < ExpandedSeedSize )
				{
					U->addNodePair(pair);
					A0i->addNodePair(pair);
					Z->addNodePair(pair);
					g1Used.insert(pair->getNodeId(graph1));
					g2Used.insert(pair->getNodeId(graph2));
				}
				else {
					keysToRemove->push_back(pair->getKey());
				}
			}
			delete newPairs;

			parallel_for(blocked_range<size_t>(0, keysToRemove->size()), RemoveUnusedPairs(keysToRemove, pairScores));
			delete keysToRemove;

			//break;
		}

		delete A;
		
		A = Z;
		//break;
	}
	delete Z;
	delete U;
	//A0i->print();

	NoisySeedsParallel noisySeeds(Graph1, Graph2, Threshold, A0i);
	auto matchedValues = noisySeeds.run(seed);

	return matchedValues;
}