#pragma once
#include <string>
#include <iostream>
#include <deque>
#include <set>

using namespace std;

namespace csr {
	class NodePair
	{
	public:
		NodePair(string g1Node,string g2Node);
		NodePair(int g1Node, int g2Node);

		std::string getKey();
		int getNodeId(int graph);
		void print();
		friend bool operator< ( NodePair &left,  NodePair &right);
	private:
		std::string g1NodeName;
		std::string g2NodeName;

		int g1NodeId;
		int g2NodeId;
	};


	class PairMatchingScore {
	public:
		PairMatchingScore(NodePair* pair);
		void Print();
		void IncremenrScore();
		int getScore();
		NodePair* getPair();
	private:
		NodePair* pair;
		int score;
	};


	class MatchedPairsSet {
	public:
		MatchedPairsSet();
		MatchedPairsSet(deque<NodePair*> pairsSet);
		~MatchedPairsSet();
		
		deque<NodePair*> getNodeSets();
		void addNodePair(NodePair* nodePair);
		void AddMatchedPairs(deque<NodePair*> matchedPairs);
		void AddMatchedPairs(MatchedPairsSet* pairsSet);

		deque<NodePair*> GetDifference(MatchedPairsSet* pairsSet);

		set<int>* getNodesForGraph(int graph);
		bool GraphContainsNode(int graph, int nodeId);

		void print();
	private:
		void MatchedPairsSet::LoadNodesForGraph(int graph);
		deque<NodePair*> NodePairs;

		set<int>* Graph1Nodes;
		set<int>* Graph2Nodes;
	};
}
