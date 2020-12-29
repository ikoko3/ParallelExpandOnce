#include <string>
#include "../Graph/Graph.h"

using namespace std;

namespace gh
{
	csr::Graph* CreateGraphFromFile(string fileName, int skipLinesCount = 0);
}