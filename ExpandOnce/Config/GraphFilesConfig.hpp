#include <string>
#include <stdexcept>


#define GRAPH_FILES_ROOT "C:\\Users\\john_\\PycharmProjects\\"

#define GRAPH_FILE1 "graph1.adjlist"
#define GRAPH_FILE2 "graph2.adjlist"

using namespace std;

#pragma once
static class GraphFilesConfig
{
public:
	const static int MAX_NODE_NAME_LENGTH = 5;
	const static int LINES_TO_SKIP = 3;


	static string get_graph_file_name(int graph_no);

		
};

