#include "CsrGraph.h"

csr::CsrGraph::CsrGraph(vector<CsrNode*> nodes, vector<string> edges)
{
	this->csrNodes = nodes;
	this->csrEdges = edges;
}
