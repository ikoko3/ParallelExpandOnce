#ifdef NDEBUG
#undef NDEBUG
#endif


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <utility>
#include <fstream>
#include <string>
#include <chrono>
#include "Graph/Graph.h"
#include "Helpers/GraphHelper.hpp"
#include "Config/GraphFilesConfig.hpp"
#include "tbb/task.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include "tbb/blocked_range.h"
#include "tbb/concurrent_vector.h"
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_hash_map.h"
#include "tbb/parallel_while.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/parallel_scan.h"
#include "tbb/pipeline.h"
#include "tbb/atomic.h"
#include "tbb/mutex.h"
#include "tbb/spin_mutex.h"
#include "tbb/queuing_mutex.h"
#include "tbb/tbb_thread.h"


using namespace std;
using namespace tbb;


static const int threshold = 2;

using namespace csr;

int main() {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	cout << "Starting Expand Once with threshold " << threshold << endl;

	Graph* graph1 = gh::CreateGraphFromFile(GraphFilesConfig::getGraphFileName(csr::graph1), GraphFilesConfig::LINES_TO_SKIP);
	Graph* graph2 = gh::CreateGraphFromFile(GraphFilesConfig::getGraphFileName(csr::graph2), GraphFilesConfig::LINES_TO_SKIP);

	SeedSet* set = gh::CreateSeedSetFromFile(GraphFilesConfig::GetNoisySeedSetName());
	auto pairScores = gh::CreateNeighbouringPairs(set, graph1, graph2);
	





	vector<int>* edges = graph1->getNeighboursFor(5);
	/*for (auto const &edge : *edges) {
		cout << edge << endl;
	}*/

	delete graph1;
	delete graph2;

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

	//parallel();
}

