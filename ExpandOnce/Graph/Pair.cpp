#include "Pair.h"
#include "Graph.h"

using namespace csr;
using namespace std;

Pair::Pair(int g1Node, int g2Node)
{
	g1NodeId = g1Node;
	g2NodeId = g2Node;
}

PairMatchingScore::PairMatchingScore(Pair* pair)
{
	this->pair = pair;
	score = 1;
}

void csr::PairMatchingScore::Print()
{
	cout << pair->getNodeId(graph1) << "|"<<pair->getNodeId(graph2) << "  --  Score:" << score << endl;
}

void csr::PairMatchingScore::IncremenrScore()
{
	score++;
}

string csr::Pair::getKey()
{
	return to_string(g1NodeId) + to_string(g2NodeId);
}

int csr::Pair::getNodeId(int graph)
{
	switch (graph) {
	case graph1:
		return g1NodeId;
	case graph2:
		return g2NodeId;
	default:
		throw graph;
	}
}
