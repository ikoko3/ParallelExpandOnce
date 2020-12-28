#include "CsrNode.h"

CsrNode::CsrNode(string name) {
	this->name = name;
	this->index = 0;
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
