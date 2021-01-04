#include "SeedSet.h"

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

void SeedSet::print()
{
	cout << endl << "-------------Printing Seed Set-------------" << endl;
	cout << "G1:(Name/Id),G2(Name:Id)" << endl;
	for(auto node : NodeSets) {
		node.print();
	}
	cout << endl;
}
