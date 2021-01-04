#include <string>
#include "GraphFilesConfig.hpp"

string GraphFilesConfig::getGraphFileName(int graph_no)
{
	string file_name;
	switch (graph_no) {
	case 1:
		file_name = string(GRAPH_FILE1);
		break;
	case 2:
		file_name = string(GRAPH_FILE2);
		break;
	default:
		throw new invalid_argument("Accepted values for graph file 1 or 2.\n");
	}
	return string(GRAPH_FILES_ROOT) + file_name;
}

string GraphFilesConfig::GetNoisySeedSetName()
{
	return string(GRAPH_FILES_ROOT) + string(NOISY_SET);
}
