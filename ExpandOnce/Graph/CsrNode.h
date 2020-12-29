#pragma once
#include <string>
#include <iostream>

using namespace std;

namespace csr {
	class CsrNode
	{
	public:
		CsrNode(string name);
		CsrNode::CsrNode(string name, int index);
		CsrNode(string name, int index, int starts, int ends);

		void print();
		int edges_count();
		void set_array_limits(int starts, int ends);
	private:
		string name;
		int index;
		int starts;
		int ends;
	};
}

