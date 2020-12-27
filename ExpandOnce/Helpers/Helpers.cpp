#include <iostream>
#include <fstream>
#include <list>  
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

		list<CsrNode*> dataSet;
		
		if (!my_file) {
			cout << "No such file";
			return;
		}
		if(!my_file.is_open()) {
			cout << "File is not open";
			return;
		}

		int line = 0;
		string tp;
		while (getline(my_file, tp)) { 
			if(line > skipLinesCount)
				dataSet.insert(dataSet.begin(), new CsrNode(tp));
			line++;
		}
		my_file.close(); 
		

		for (auto const& i : dataSet) {
			cout << i->Name <<"\n";
		}
	}
}