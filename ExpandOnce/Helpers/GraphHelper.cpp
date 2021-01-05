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
#include "../Graph/Pair.h"



using namespace std;
using namespace csr;

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

	csr::Graph* CreateGraphFromFile(string fileName, int skipLinesCount)
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

	SeedSet * CreateSeedSetFromFile(string fileName)
	{
		fstream setFile = OpenFile(fileName);
		deque<NodeSet> nodeSets;

		string tp;
		while (getline(setFile, tp)) {
			//We know the format is node1 node2, so we split the values and create a new item on each row
			istringstream iss(tp);
			vector<string> matchedNodes{ istream_iterator<string>{iss}, istream_iterator<string>{} };
			nodeSets.push_back(NodeSet(matchedNodes[0],matchedNodes[1]));

			//cout << tp << endl;
	
		}
		setFile.close();

		return new SeedSet(nodeSets);
	}

	map<string, PairMatchingScore*> CreateNeighbouringPairs(SeedSet *set, Graph* g1, Graph* g2)
	{
		map<string,PairMatchingScore*> pairScores;

		for (auto &nodeSet : set->getNodeSets()) {
			vector<int>* g1edges = g1->getNeighboursFor(nodeSet.getNodeId(graph1));
			vector<int>* g2edges = g1->getNeighboursFor(nodeSet.getNodeId(graph2));

			for (auto &g1edge : *g1edges) {
				for (auto &g2edge : *g2edges) {
					auto pair = new Pair(g1edge,g2edge);
					auto pairKey = pair->getKey();

					auto it = pairScores.find(pairKey);
					PairMatchingScore* pairScore;

					if (it == pairScores.end()) {
						pairScore = new PairMatchingScore(pair);
						pairScores[pairKey] = pairScore;
					}
					else {
						pairScore = it->second;
						pairScore->IncremenrScore();
					}
				
					//pairScore->Print();
				}
			}

			delete g1edges;
			delete g2edges;
		}

		
	}


}