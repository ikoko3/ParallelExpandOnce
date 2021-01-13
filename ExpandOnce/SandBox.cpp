#include "SandBox.h"

#include "Algorithm\NoisySeeds.h"
#include "Helpers/GraphHelper.hpp"

#include "tbb/concurrent_hash_map.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include <string>
#include <iostream>

using namespace tbb;
using namespace std;

// Structure that defines hashing and comparison operations for user's type.
struct MyHashCompare {
	static size_t hash(const string& x) {
		size_t h = 0;
		for (const char* s = x.c_str(); *s; ++s)
			h = (h * 17) ^ *s;
		return h;
	}
	//! True if strings are equal
	static bool equal(const string& x, const string& y) {
		return x == y;
	}
};

// A concurrent hash table that maps strings to ints.
typedef concurrent_hash_map<string, int, MyHashCompare> MyMap;

// Function object for counting occurrences of strings.
struct Tally {
	MyMap& table;
	Tally(MyMap& table_) : table(table_) {}
	void operator()(const blocked_range<string*> range) const {
		for (string* p = range.begin(); p != range.end(); ++p) {
			MyMap::accessor a;
			table.insert(a, *p);
			a->second += 1;
		}
	}
};

const size_t N = 1000000;

string Data[N];


void CountOccurrences() {
	// Construct empty table.
	MyMap table;

	// Put occurrences into the table
	parallel_for(blocked_range<string*>(Data, Data + N, 100),
		Tally(table));

	// Display the occurrences
	for (MyMap::iterator i = table.begin(); i != table.end(); ++i)
		printf("%s %d\n", i->first.c_str(), i->second);
}



void Foo(int t) {
	int worker_index = tbb::task_arena::current_thread_index();
	printf("%d-%d\n",t,worker_index);
}

class ApplyVotes {
	int *const _g1edges;
public:
	void operator()(const blocked_range<size_t>& r) const {
		int *a = _g1edges;
		for (size_t i = r.begin(); i != r.end(); ++i)
			Foo(a[i]);
	}
	ApplyVotes(int a[]) :
		_g1edges(a)
	{}
};

void ParallelApplyFoo(int a[], size_t n) {
	parallel_for(blocked_range<size_t>(0, n), ApplyVotes(a));
}

void main1() {
	//tbb::task_scheduler_init init(8);
	//task_scheduler_init init(tbb::task_scheduler_init::automatic);

	//int a[1000];
	//for (int i = 0; i < 1000; i++) { a[i] = i; }

	//ParallelApplyFoo(a, 1000);


}



