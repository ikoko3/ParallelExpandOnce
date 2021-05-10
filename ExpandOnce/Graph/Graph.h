#pragma once
#include <vector>
#include "Node.h"

using namespace std;

namespace csr {
	class Graph
	{
	public:
		Graph(vector<Node*> nodes, vector<int> edges);
		~Graph();

		vector<int>* getNeighboursFor(int nodeIndex);
		int getNodesCount();
		int getNodesCount(int edges); // Get nodes with at least n edges

		int getEdgesCount();
	private:
		vector<Node*> nodes;
		vector<int> edges;
	};

	enum NodeGraph {
		graph1 = 1,
		graph2 = 2
	};
}
