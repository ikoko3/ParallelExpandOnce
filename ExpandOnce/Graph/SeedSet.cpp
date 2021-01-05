#include "SeedSet.h"
#include "Graph.h"

using namespace csr;

NodeSet::NodeSet()
{
}

NodeSet::NodeSet(string g1Name, string g2Name)
{
	g1NodeName = g1Name;
	g2NodeName = g2Name;

	g1NodeId = stoi(g1Name);
	g2NodeId = stoi(g2Name);
}

int NodeSet::getNodeId(int graph)
{
	switch (graph) {
	case graph1:
		return g1NodeId;
	case graph2:
		return g2NodeId;
	default:
		throw graph;
	}

	return 0;
}

void NodeSet::print()
{
	cout << "G1:(" << g1NodeName << "/" << g1NodeId << "),G2:(" << g2NodeName << "/" << g2NodeId << ")" << endl;
}


SeedSet::SeedSet()
{
}

SeedSet::SeedSet(deque<NodeSet> nodeSets)
{
	NodeSets = nodeSets;
}

deque<NodeSet> SeedSet::getNodeSets()
{
	return NodeSets;
}

void SeedSet::print()
{
	cout << endl << "-------------Printing Seed Set-------------" << endl;
	cout << "G1:(Name/Id),G2(Name:Id)" << endl;
	for(auto node : NodeSets) {
		node.print();
	}
	cout << endl;
}
