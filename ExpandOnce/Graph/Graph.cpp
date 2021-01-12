#include "Graph.h"

csr::Graph::Graph(vector<Node*> nodes, vector<int> edges)
{
	this->nodes = nodes;
	this->edges = edges;
}

csr::Graph::~Graph()
{
	for (auto const& node : nodes) {
		delete node;
	}
}

vector<int>* csr::Graph::getNeighboursFor(int nodeIndex)
{
	//cout << nodes.size() - nodeIndex  << endl;
	auto const node = nodes[nodes.size() - nodeIndex -1]; //The indexes are reversed
	vector<int>::const_iterator first = edges.begin() + node->get_starts();
	vector<int>::const_iterator last = edges.begin() + node->get_ends();
	
	return new vector<int>(first, last);
}
