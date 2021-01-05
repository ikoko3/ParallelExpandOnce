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
	graph1Nodes = nullptr;
	graph2Nodes = nullptr;
}

csr::SeedSet::~SeedSet()
{
	if (graph1Nodes != nullptr) delete graph1Nodes;
	if (graph2Nodes != nullptr) delete graph2Nodes;
}

SeedSet::SeedSet(deque<NodeSet> nodeSets) 
{
	graph1Nodes = nullptr;
	graph2Nodes = nullptr;
	NodeSets = nodeSets;
}

deque<NodeSet> SeedSet::getNodeSets()
{
	return NodeSets;
}

void SeedSet::LoadNodesForGraph(int graph) {
	auto nodes = new set<int>;
	for (auto nodeSet : NodeSets) {
		nodes->insert(nodeSet.getNodeId(graph));
	}

	switch (graph) {
	case graph1:
		graph1Nodes = nodes;
		break;
	case graph2:
		graph2Nodes = nodes;
		break;
	default:
		throw graph;
	}	
}


set<int>* SeedSet::getNodesForGraph(int graph)
{
	switch (graph) {
	case graph1:
		if (graph1Nodes == nullptr)
			LoadNodesForGraph(graph1);
		return graph1Nodes;
	case graph2:
		if (graph2Nodes == nullptr)
			LoadNodesForGraph(graph2);
		return graph1Nodes;
	default:
		throw graph;
	}
}

bool SeedSet::GraphContainsNode(int graph, int nodeId)
{
	return getNodesForGraph(graph)->count(nodeId) > 0;
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
