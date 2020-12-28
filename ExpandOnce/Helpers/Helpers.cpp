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

namespace gh
{
	int test() {
		std::cout << "hey hey";
		return 0;
	}

	void open_graph_file(string file_name) {
		fstream my_file;
		my_file.open(file_name, ios::in);


		if (!my_file) {
			cout << "No such file";
		}
		else if (my_file.is_open()) {
			string tp;
			while (getline(my_file, tp)) {
				cout << tp << "\n";
			}
			my_file.close();
		}
	}

	void open_file_test(string file_name, int skipLinesCount =0 ) {
		std::cout << "FileName: " + file_name << endl;
		fstream my_file;
		my_file.open(file_name, ios::in);

		vector<CsrNode*> csrNodes;
		vector<string> csrEdges;
		
		if (!my_file) {
			cout << "No such file";
			return;
		}
		if(!my_file.is_open()) {
			cout << "File is not open";
			return;
		}

		int fileLine = 0;
		int edgesCount = 0;
		int nodesCount = 0;
		string tp;
		while (getline(my_file, tp)) { 
			if (fileLine++ < skipLinesCount)
				continue;

			//Split values to seperate strings
			istringstream iss(tp);
			deque<string> node_edges{ istream_iterator<string>{iss}, istream_iterator<string>{} };


			string nodeName = node_edges[0];
			node_edges.pop_front(); //Remove the first item because it is the node name
			csrEdges.insert(csrEdges.end(), node_edges.begin(),node_edges.end());
	
			csrNodes.insert(csrNodes.begin(), new CsrNode(nodeName,nodesCount++,edgesCount,edgesCount + node_edges.size() -1));
			edgesCount += node_edges.size();
		}
		my_file.close(); 
		

		for (auto const& node : csrNodes) {
			node->print();
		}

		//Dispose
		for (auto const& node : csrNodes) {
			delete node;
		}

		for (auto const& edge : csrEdges) {
			cout << edge << "-";
		}
	}


}