#pragma once
#include <string>
#include <iostream>

using namespace std;

namespace csr {
	class Node
	{
	public:
		Node(string name);
		Node::Node(string name, int index);
		Node(string name, int index, int starts, int ends);

		void print();
		int edges_count();
		void set_array_limits(int starts, int ends);
		int get_starts();
		int get_ends();
	private:
		string name;
		int index;
		int starts;
		int ends;
	};
}

