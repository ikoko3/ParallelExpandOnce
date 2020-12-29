#include <string>
#include "../Graph/CsrGraph.h"

using namespace std;

namespace gh
{
	csr::CsrGraph CreateGraphFromFile(string fileName, int skipLinesCount = 0);
}