#include <string>
#include "../Graph/Graph.h"
#include "../Graph/SeedSet.h"

using namespace std;

namespace gh
{
	csr::Graph* CreateGraphFromFile(string fileName, int skipLinesCount = 0);
	csr::SeedSet* CreateSeedSetFromFile(string fileName);
}