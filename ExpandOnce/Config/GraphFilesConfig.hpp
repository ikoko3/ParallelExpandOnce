#include <string>
#include <stdexcept>


#define GRAPH_FILES_ROOT "C:\\Users\\john_\\Documents\\tbb_env\\tbb\\"

#define GRAPH_FILE1 "graph1.adjlist"
#define GRAPH_FILE2 "graph2.adjlist"
#define NOISY_SET "noisy_set.list"

using namespace std;

#pragma once
static class GraphFilesConfig
{
public:
	const static int MAX_NODE_NAME_LENGTH = 5;
	const static int LINES_TO_SKIP = 3;


	static string getGraphFileName(int graph_no);

	static string getNoisySeedSetName();
};

