#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <vector>
#include <deque>

using namespace std;

namespace csr {
	class NodeSet {
	public:
		NodeSet();
		NodeSet(string g1Name, string g2Name);

		int getNodeId(int graph);
		void print();
	private:
		string g1NodeName;
		string g2NodeName;

		int g1NodeId;
		int g2NodeId;
	};


	class SeedSet{
	public:
		SeedSet();
		~SeedSet();
		SeedSet(deque<NodeSet> nodeSets);
		deque<NodeSet> getNodeSets();
		set<int>* getNodesForGraph(int graph);
		bool GraphContainsNode(int graph, int nodeId);

		void print();
	private:
		void SeedSet::LoadNodesForGraph(int graph);
		deque<NodeSet> NodeSets;
		set<int>* graph1Nodes;
		set<int>* graph2Nodes;
	};
}
