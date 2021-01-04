#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <vector>
#include <deque>

using namespace std;

namespace csr {
	class NodeSet {
	public:
		NodeSet();
		NodeSet(string g1Name, string g2Name);
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
		SeedSet(deque<NodeSet> nodeSets);
		void print();
	private:
		deque<NodeSet> NodeSets;
	};
}
