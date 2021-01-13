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
#include "SandBox.h"
#include "Helpers/GraphHelper.hpp"
#include "Config/GraphFilesConfig.hpp"
#include "Algorithm/NoisySeeds.h"
#include "Algorithm/ExpandOnce.h"
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


static const int threshold = 6;
static const int seedSize = 9;

using namespace csr;



int main() {


	Graph* graph1 = gh::createGraphFromFile(GraphFilesConfig::getGraphFileName(csr::graph1), GraphFilesConfig::LINES_TO_SKIP);
	Graph* graph2 = gh::createGraphFromFile(GraphFilesConfig::getGraphFileName(csr::graph2), GraphFilesConfig::LINES_TO_SKIP);
	MatchedPairsSet* set = gh::createSeedSetFromFile(GraphFilesConfig::getNoisySeedSetName());

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//alg::ExpandOnceSerial expandOnce(graph1, graph2, threshold, seedSize, set);
	//auto matchedValues = expandOnce.run();

	alg::NoisySeedsParallel noisySeeds(graph1, graph2, threshold, set);
	auto matchedValues = noisySeeds.run();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	
	//matchedValues->print();
	//matchedValues->printAccuracy();
	std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;


	delete matchedValues;
	delete set;
	delete graph1;
	delete graph2;	
}

