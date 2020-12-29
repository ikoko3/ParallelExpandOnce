#include "CsrNode.h"

using namespace csr;

CsrNode::CsrNode(string name) {
	this->name = name;
	this->index = 0;
	this->starts = 0;
	this->ends = 0;
}

CsrNode::CsrNode(string name, int index)
{
	this->name = name;
	this->index = index;
	this->starts = 0;
	this->ends = 0;
}


CsrNode::CsrNode(string name, int index, int starts, int ends)
{
	this->name = name;
	this->index = index;
	this->starts = starts;
	this->ends = ends;
}

void CsrNode::print() {
	cout << "Node:"<< name << ", idx:" << index << ", count:"  << edges_count() <<", pos:" <<starts<<"/"<<ends <<endl;
}

int CsrNode::edges_count()
{
	return ends-starts + 1;
}

void CsrNode::set_array_limits(int starts, int ends)
{
	this->starts = starts;
	this->ends = ends;
}
