#include "GraphHelper.hpp"
#include <iostream>
#include <fstream>
#include <list>  
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <deque>
#include <string>
#include "../Graph/CsrNode.h"


using namespace std;
using namespace csr;

namespace gh
{

	csr::CsrGraph CreateGraphFromFile(string fileName, int skipLinesCount)
	{
		std::cout << "Creating graph from file: " + fileName << endl;
		fstream graphFile;
		graphFile.open(fileName, ios::in);

		vector<CsrNode*> csrNodes;
		vector<string> csrEdges;
		vector<deque<string>> edgesDeques;

		if (!graphFile) {
			cout << "No such file";
			throw (graphFile);
		}
		if (!graphFile.is_open()) {
			cout << "File is not open";
			throw (graphFile);
		}

		int fileLine = 0;
		int edgesCount = 0;
		int nodesCount = 0;
		string tp;
		while (getline(graphFile, tp)) {
			if (fileLine++ < skipLinesCount)
				continue;

			//Split values to seperate strings
			istringstream iss(tp);
			deque<string> node_edges{ istream_iterator<string>{iss}, istream_iterator<string>{} };


			string nodeName = node_edges[0];
			node_edges.pop_front(); //Remove the first item because it is the node name
			edgesDeques.push_back(node_edges);

			csrNodes.insert(csrNodes.begin(), new CsrNode(nodeName, nodesCount++));
		}
		graphFile.close();

		//Complete each node with missing nodes
		for (int i = csrNodes.size() - 1; i >= 0; i--) {
			auto const node = csrNodes[i];

			int vectorIndex = csrNodes.size() - 1 - i;
			auto const node_edges = edgesDeques[vectorIndex];


			for (auto const& edge : node_edges) {
				int edgeIndex = stoi(edge); // we can 
				edgesDeques[edgeIndex].push_back(to_string(vectorIndex));
			}
			node->set_array_limits(edgesCount, edgesCount + node_edges.size() - 1);
			edgesCount += node_edges.size();

			csrEdges.insert(csrEdges.end(), node_edges.begin(), node_edges.end());
		}
		return csr::CsrGraph(csrNodes, csrEdges);
	}


}