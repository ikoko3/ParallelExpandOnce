#include "NodePair.h"
#include "Graph.h"

#include <stdlib.h>
#include <map>
#include <algorithm>

using namespace csr;
using namespace std;

NodePair::NodePair(string g1Name, string g2Name)
{
	g1NodeId = stoi(g1Name);
	g2NodeId = stoi(g2Name);
}

csr::NodePair::NodePair(int g1Node, int g2Node)
{
	g1NodeId = g1Node;
	g2NodeId = g2Node;
}

PairMatchingScore::PairMatchingScore(NodePair* pair)
{
	this->pair = pair;
	score = 1;
}

csr::PairMatchingScore::~PairMatchingScore()
{
	delete pair;
}

void PairMatchingScore::print()
{
	cout << pair->getNodeId(graph1) << "|"<<pair->getNodeId(graph2) << "  --  Score:" << score << endl;
}

void PairMatchingScore::incrementScore()
{
	score++;
}

int PairMatchingScore::getScore()
{
	return score;
}

int csr::PairMatchingScore::getPairsDistance()
{
	return abs(pair->getNodeId(graph1) - pair->getNodeId(graph2));
}

NodePair * PairMatchingScore::getPair()
{
	return pair;
}

string csr::PairMatchingScore::getPairKey()
{
	return pair->getKey();
}


string NodePair::getKey()
{
	return to_string(g1NodeId) + "-"+ to_string(g2NodeId);
}

int NodePair::getNodeId(int graph)
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

void NodePair::print()
{
	cout << "G1:(" <<  g1NodeId << "),G2:(" << g2NodeId << ")" << endl;
}

bool csr::NodePair::valuesAreMatched()
{
	return g1NodeId == g2NodeId;
}


MatchedPairsSet::MatchedPairsSet()
{

}

MatchedPairsSet::MatchedPairsSet(deque<NodePair*> pairsSet)
{
	NodePairs = pairsSet;
}

MatchedPairsSet::MatchedPairsSet(MatchedPairsSet* pairsSet)
{
	addMatchedPairs(pairsSet);
}

MatchedPairsSet::~MatchedPairsSet()
{
	for (auto pair : NodePairs) {
		delete pair;
	}
}

deque<NodePair*> MatchedPairsSet::getNodeSets()
{
	return NodePairs;
}

void MatchedPairsSet::addNodePair(NodePair* nodePair)
{
	NodePairs.push_back(new NodePair(*nodePair));
	Graph1Nodes.insert(nodePair->getNodeId(graph1));
	Graph2Nodes.insert(nodePair->getNodeId(graph2));
}

void MatchedPairsSet::addMatchedPairs(deque<NodePair*> matchedPairs)
{
	for (auto const pair : matchedPairs)
		addNodePair(pair);
}

void MatchedPairsSet::addMatchedPairs(MatchedPairsSet* pairsSet)
{
	addMatchedPairs(pairsSet->getNodeSets());
}

set<NodePair*>* GetSetFromDeque(deque<NodePair*> nodePairs) {
	set<NodePair*>* pairsSet = new set<NodePair*>();
	for (auto pair : nodePairs) 
		pairsSet->insert(pair);
	
	return pairsSet;
}

deque<NodePair*>* MatchedPairsSet::getDifference(MatchedPairsSet * pairsSet)
{
	map<string, NodePair*>* existingPairs = new map<string, NodePair*>();
	for (auto pair : NodePairs) {
		(*existingPairs)[pair->getKey()] = pair;
	}
	deque<NodePair*>* pairsDifference = new deque<NodePair*>;

	for (auto pair : pairsSet->getNodeSets()) {
		auto it = existingPairs->find(pair->getKey());
		if (it == existingPairs->end())
			pairsDifference->push_back(pair);
	}

	delete existingPairs;
	return pairsDifference;
}


set<int>* MatchedPairsSet::getNodesForGraph(int graph)
{
	auto nodes = new set<int>;
	for (auto nodeSet : NodePairs) 
		nodes->insert(nodeSet->getNodeId(graph));
	
	return nodes;
}

bool MatchedPairsSet::graphContainsNode(int graph, int nodeId)
{
	switch (graph) {
	case graph1:
		return Graph1Nodes.find(nodeId) != Graph1Nodes.end();
	case graph2:
		return Graph2Nodes.find(nodeId) != Graph2Nodes.end();
	}
}

SetAccuraccy* csr::MatchedPairsSet::getAccuracy()
{
	int  correct = 0;
	for (auto node : NodePairs) {
		if (node->valuesAreMatched())
			correct++;
	}
	float accuracy = (float)correct / (float)NodePairs.size();

	return new SetAccuraccy(correct, NodePairs.size(), accuracy);
}

void MatchedPairsSet::print()
{
	cout << endl << "-------------Printing Matched Pairs Set-------------" << endl;
	cout << "G1:(Id),G2:(Id)" << endl;
	for (auto node : NodePairs) {
		node->print();
	}
	cout << endl;
}

void csr::MatchedPairsSet::printAccuracy()
{
	auto accuracy = getAccuracy();
	cout <<"Accuracy: "<< accuracy->accuraccy*100 << "% ("<< accuracy->correct <<"/"<< NodePairs.size()<<")"<< endl;
	delete accuracy;
}

bool csr::operator<( NodePair  &left,  NodePair &right)
{
	return left.getKey() < right.getKey();
}

bool csr::compareScores(PairMatchingScore * s1, PairMatchingScore * s2)
{
	if (s1->getScore() == s2->getScore()) {
		return s1->getPairsDistance() < s2->getPairsDistance();
	}

	return (s1->getScore() < s2->getScore());
}
