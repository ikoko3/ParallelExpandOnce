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
		bool valuesAreMatched();
	private:
		std::string g1NodeName;
		std::string g2NodeName;

		int g1NodeId;
		int g2NodeId;
	};


	class PairMatchingScore {
	public:
		PairMatchingScore(NodePair* pair);
		~PairMatchingScore();

		void print();
		void incrementScore();
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
		MatchedPairsSet(MatchedPairsSet* pairsSet);
		~MatchedPairsSet();
		
		deque<NodePair*> getNodeSets();
		void addNodePair(NodePair* nodePair);
		void addMatchedPairs(deque<NodePair*> matchedPairs);
		void addMatchedPairs(MatchedPairsSet* pairsSet);

		deque<NodePair*> getDifference(MatchedPairsSet* pairsSet);

		set<int>* getNodesForGraph(int graph);
		bool graphContainsNode(int graph, int nodeId);

		void print();
		void printAccuracy();
	private:
		deque<NodePair*> NodePairs;

		set<int> Graph1Nodes;
		set<int> Graph2Nodes;
	};
}
