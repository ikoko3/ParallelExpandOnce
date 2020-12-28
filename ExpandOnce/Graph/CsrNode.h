#pragma once
#include <string>
#include <iostream>

using namespace std;

class CsrNode 
{
public:
	CsrNode(string name);
	CsrNode(string name, int index, int starts, int ends);

	void print();
	int edges_count();
private:
	string name;
	int index;
	int starts;
	int ends;
};

