#pragma once
#include <string>
#include <iostream>

namespace csr {
	class Pair
	{
	public:
		Pair(int g1Node,int g2Node);
		std::string getKey();
		int getNodeId(int graph);
	private:
		int g1NodeId;
		int g2NodeId;
	};

	class PairMatchingScore {
	public:
		PairMatchingScore(Pair* pair);
		void Print();
		void IncremenrScore();
		int getScore();
		Pair* getPair();
	private:
		Pair* pair;
		int score;
	};
}
