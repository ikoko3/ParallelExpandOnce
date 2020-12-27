/*
    ==============================================================

    SAMPLE SOURCE CODE - SUBJECT TO THE TERMS OF SAMPLE CODE LICENSE AGREEMENT,
    http://software.intel.com/en-us/articles/intel-sample-source-code-license-agreement/

    Copyright 2005-2019 Intel Corporation

    THIS FILE IS PROVIDED "AS IS" WITH NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
    NOT LIMITED TO ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    PURPOSE, NON-INFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS.

    =============================================================
*/

/* Example program that computes Fibonacci numbers in different ways.
   Arguments are: [ Number [Threads [Repeats]]]
   The defaults are Number=500 Threads=1:4 Repeats=1.

   The point of this program is to check that the library is working properly.
   Most of the computations are deliberately silly and not expected to
   show any speedup on multiprocessors.
*/

// enable assertions
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
#include "Helpers/Helpers.hpp"
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


int main() {

	gh::open_file_test(GraphFilesConfig::get_graph_file_name(1),GraphFilesConfig::LINES_TO_SKIP);
	gh::open_file_test(GraphFilesConfig::get_graph_file_name(2),GraphFilesConfig::LINES_TO_SKIP);
	

	//parallel();
}

