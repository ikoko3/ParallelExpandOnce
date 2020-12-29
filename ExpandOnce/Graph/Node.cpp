#include "Node.h"

using namespace csr;

Node::Node(string name) {
	this->name = name;
	this->index = 0;
	this->starts = 0;
	this->ends = 0;
}

Node::Node(string name, int index)
{
	this->name = name;
	this->index = index;
	this->starts = 0;
	this->ends = 0;
}


Node::Node(string name, int index, int starts, int ends)
{
	this->name = name;
	this->index = index;
	this->starts = starts;
	this->ends = ends;
}

void Node::print() {
	cout << "Node:"<< name << ", idx:" << index << ", count:"  << edges_count() <<", pos:" <<starts<<"/"<<ends <<endl;
}

int Node::edges_count()
{
	return ends-starts + 1;
}

void Node::set_array_limits(int starts, int ends)
{
	this->starts = starts;
	this->ends = ends;
}

int csr::Node::get_starts()
{
	if(edges_count() == 0)
		return 0;
	return starts;
}

int csr::Node::get_ends()
{
	if (edges_count() == 0)
		return 0;
	return ends;
}
