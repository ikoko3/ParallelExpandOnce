#pragma once
#include <vector>
#include "CsrNode.h"

using namespace std;

namespace csr {
	class CsrGraph
	{
	public:
		CsrGraph(vector<CsrNode*> nodes, vector<string> edges);

	private:
		vector<CsrNode*> csrNodes;
		vector<string> csrEdges;
	};


}
