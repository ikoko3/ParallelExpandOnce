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


static const size_t N = 23;
static const int threshold = 2;

class SubStringFinder {

	const string str;
	size_t * max_array;
	size_t * pos_array;

public:

		void operator() (const blocked_range<size_t>& r) const {
		for (size_t i = r.begin(); i != r.end(); ++i) {
			size_t max_size = 0, max_pos = 0;
			for (size_t j = 0; j < str.size(); ++j)
				if (j != i) {
					size_t limit = str.size() - max(i, j);
					for (size_t k = 0; k < limit; ++k) {
						if (str[i + k] != str[j + k]) break;
						if (k > max_size) {
							max_size = k;
							max_pos = j;
						}
					}
				}

			max_array[i] = max_size;
			pos_array[i] = max_pos;
		}
		
	}

	SubStringFinder(string &s, size_t *m, size_t *p) :
		str(s), max_array(m), pos_array(p) { }


};

int serial() {
	string str[N] = { string("a"), string("b") };
	for (size_t i = 2; i < N; ++i) str[i] = str[i - 1] + str[i - 2];
	string & to_scan = str[N - 1];
	size_t num_elem = to_scan.size();
	size_t * max = new size_t[num_elem];


	size_t * pos = new size_t[num_elem];


	// add code to populate max and pos here

	for (size_t i = 0; i < num_elem; ++i)
		cout << " " << max[i] << "(" << pos[i] << ")" << endl;

	delete[] pos;


	delete[] max;
	return 0;

}

int parallel() {

	string str[N] = { string("a"), string("b") };
	for (size_t i = 2; i < N; ++i) str[i] = str[i - 1] + str[i - 2];
	string & to_scan = str[N - 1];

	size_t num_elem = to_scan.size();

	size_t * max = new size_t[num_elem];
	size_t * pos = new size_t[num_elem];



	parallel_for(blocked_range<size_t>(0, num_elem),
		SubStringFinder(to_scan, max, pos));

	for (size_t i = 0; i < num_elem; ++i)
		cout << " " << max[i] << "(" << pos[i] << ")" << endl;
	delete[] pos;
	delete[] max;
	return 0;

}

using namespace csr;

int main() {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	cout << "Starting Expand Once with threshold " << threshold << endl;

	Graph* graph1 = gh::CreateGraphFromFile(GraphFilesConfig::getGraphFileName(csr::graph1), GraphFilesConfig::LINES_TO_SKIP);
	Graph* graph2 = gh::CreateGraphFromFile(GraphFilesConfig::getGraphFileName(csr::graph2), GraphFilesConfig::LINES_TO_SKIP);

	SeedSet* set = gh::CreateSeedSetFromFile(GraphFilesConfig::GetNoisySeedSetName());
	set->print();



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

