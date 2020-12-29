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
	private:
		vector<Node*> nodes;
		vector<int> edges;
	};


}
