#include "GraphHelper.hpp"
#include <iostream>
#include <fstream>
#include <list>  
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <deque>
#include <string>
#include <map>
#include "../Graph/Node.h"
#include "../Graph/NodePair.h"
#include "tbb/concurrent_hash_map.h"
#include "tbb/concurrent_vector.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"



using namespace std;
using namespace csr;
using namespace tbb;

namespace gh
{
	fstream OpenFile(string fileName) {
		fstream file;
		file.open(fileName, ios::in);

		if (!file) {
			cout << "No such file";
			throw (file);
		}
		if (!file.is_open()) {
			cout << "File is not open";
			throw (file);
		}
		return file;
	}

	csr::Graph* createGraphFromFile(string fileName, int skipLinesCount)
	{
		std::cout << "Creating graph from file: " + fileName << endl;
	
		vector<Node*> csrNodes;
		vector<int> csrEdges;
		vector<deque<int>> edgesDeques;

		fstream graphFile = OpenFile(fileName);

		int fileLine = 0;
		int edgesCount = 0;
		int nodesCount = 0;
		string tp;
		while (getline(graphFile, tp)) {
			if (fileLine++ < skipLinesCount)
				continue;

			//Split values to seperate strings
			istringstream iss(tp);
			deque<int> node_edges{ istream_iterator<int>{iss}, istream_iterator<int>{} };


			int nodeName = node_edges.front();
			node_edges.pop_front(); //Remove the first item because it is the node name
			edgesDeques.push_back(node_edges);

			csrNodes.insert(csrNodes.begin(), new Node(to_string(nodeName), nodesCount++));
		}
		graphFile.close();

		//Complete each node with missing nodes
		for (int i = csrNodes.size() - 1; i >= 0; i--) {
			auto const node = csrNodes[i];

			int vectorIndex = csrNodes.size() - 1 - i;
			auto const node_edges = edgesDeques[vectorIndex];


			for (auto const& edge : node_edges) {
				int edgeIndex = edge; // we know the index of the node is the name of the node
				edgesDeques[edgeIndex].push_back(vectorIndex);
			}
			node->set_array_limits(edgesCount, edgesCount + node_edges.size() - 1);
			edgesCount += node_edges.size();

			csrEdges.insert(csrEdges.end(), node_edges.begin(), node_edges.end());
		}
		
		std::cout << "Created graph from file: " + fileName << endl;
		return new csr::Graph(csrNodes, csrEdges);
	}

	MatchedPairsSet * createSeedSetFromFile(string fileName)
	{
		fstream setFile = OpenFile(fileName);
		deque<NodePair*> nodeSets;

		string tp;
		while (getline(setFile, tp)) {
			//We know the format is node1 node2, so we split the values and create a new item on each row
			istringstream iss(tp);
			vector<string> matchedNodes{ istream_iterator<string>{iss}, istream_iterator<string>{} };
			nodeSets.push_back(new NodePair(matchedNodes[0],matchedNodes[1]));

			//cout << tp << endl;
	
		}
		setFile.close();

		return new MatchedPairsSet(nodeSets);
	}

	void createNeighbouringPairs(deque<NodePair*> nodePairs, Graph* g1, Graph* g2, map<string, PairMatchingScore*>* pairScores)
	{
		for (auto &nodeSet : nodePairs) 
			createNeighbouringPairs(nodeSet, g1, g2, pairScores);
		
	}

	void createNeighbouringPairs(NodePair * nodePair, Graph * g1, Graph * g2, map<string, PairMatchingScore*>* pairScores)
	{
		vector<int>* g1edges = g1->getNeighboursFor(nodePair->getNodeId(graph1));
		vector<int>* g2edges = g2->getNeighboursFor(nodePair->getNodeId(graph2));

		for (auto &g1edge : *g1edges) {
			for (auto &g2edge : *g2edges) {
				auto pair = new NodePair(g1edge, g2edge);
				auto pairKey = pair->getKey();

				auto it = pairScores->find(pairKey);
				PairMatchingScore* pairScore;

				if (it == pairScores->end()) {
					pairScore = new PairMatchingScore(pair);
					(*pairScores)[pairKey] = pairScore;
				}
				else {
					pairScore = it->second;
					pairScore->incrementScore();
				}

				//pairScore->print();
			}
		}

		delete g1edges;
		delete g2edges;
	}

	void removeUnusedNeighbouringPairs(Graph * g1, Graph * g2, map<string, PairMatchingScore*>* pairScores, MatchedPairsSet* M)
	{
		list<string> keysToRemove;

		for (auto it = pairScores->cbegin(); it != pairScores->cend() /* not hoisted */; /* no increment */)
		{
			auto pairScore = it->second;
			if (M->graphContainsNode(graph1, pairScore->getPair()->getNodeId(graph1))
				|| M->graphContainsNode(graph2, pairScore->getPair()->getNodeId(graph2))) {
				delete it->second;
				it = pairScores->erase(it);
			}
				
			else
				++it;
			
		}

	}

	void voteNeighbouringEdges(int g1edge, vector<int>* g2edges, PairScores* pairScores) {

		for (auto &g2edge : *g2edges) {
			auto pair = new NodePair(g1edge, g2edge);
			auto pairKey = pair->getKey();

			PairScores::accessor acc;
			auto found = pairScores->find(acc, pairKey);
			if (!found) {
				pairScores->insert(acc, pairKey);
				acc->second = new PairMatchingScore(pair);;
			}
			else {
				acc->second->incrementScore();
			}
		}
	}


	struct ApplyVotes {
		vector<int>* const _g1edges;
		vector<int>* const _g2edges;
		PairScores* const _pairScores;
	public:
		void operator()(const blocked_range<size_t>& r) const {
			vector<int>* edges = _g1edges;
			for (size_t i = r.begin(); i != r.end(); ++i)
				voteNeighbouringEdges((*edges)[i], _g2edges, _pairScores);
		}
		ApplyVotes(vector<int>* g1edges, vector<int>* g2edges, PairScores* pairScores) :
			_g1edges(g1edges), _g2edges(g2edges), _pairScores(pairScores) {}
	};

	void ParallelVoteNeighbours(vector<int>* g1edges, size_t n, vector<int>* g2edges, PairScores* pairScores) {
		parallel_for(blocked_range<size_t>(0, n), ApplyVotes(g1edges, g2edges, pairScores));
	}


	void createNeighbouringPairs(NodePair * nodePair, Graph * g1, Graph * g2, PairScores* pairScores)
	{
		PairScores table;

		vector<int>* g1edges = g1->getNeighboursFor(nodePair->getNodeId(graph1));
		vector<int>* g2edges = g1->getNeighboursFor(nodePair->getNodeId(graph2));

		ParallelVoteNeighbours(g1edges, g1edges->size(), g2edges, pairScores);

		delete g1edges;
		delete g2edges;
	}

	void createNeighbouringPairs(deque<NodePair*> nodePairs, Graph* g1, Graph* g2, PairScores* pairScores)
	{
		for (auto &nodeSet : nodePairs)
			createNeighbouringPairs(nodeSet, g1, g2, pairScores);

	}

	void removeKey(string key, PairScores* pairScores) {
		PairScores::accessor acc;
		auto found = pairScores->find(acc, key);

		if (found) {
			delete acc->second;
			pairScores->erase(acc);
		}

	}

	

}


