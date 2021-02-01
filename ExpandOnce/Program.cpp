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



using namespace std;
using namespace tbb;
using namespace csr;
using namespace chrono;

int threshold = 6;
int seedSize = 10;

void parseArguements(int argc, char* argv[]) {	
	switch (argc) {
	case 3:
		seedSize = atoi(argv[2]);
	case 2:
		threshold = atoi(argv[1]);
	case 1:
	default:
		break;
	}
}



void compareNoisySeedsImplementations() {
	Graph* graph1 = gh::createGraphFromFile(FilesConfig::getGraphFileName(csr::graph1), FilesConfig::LINES_TO_SKIP);
	Graph* graph2 = gh::createGraphFromFile(FilesConfig::getGraphFileName(csr::graph2), FilesConfig::LINES_TO_SKIP);
	MatchedPairsSet* set = gh::createSeedSetFromFile(FilesConfig::getNoisySeedSetName());

	steady_clock::time_point beginS = steady_clock::now();
	alg::NoisySeedsSerial noisySeeedsS(graph1, graph2, threshold, set);
	auto matchedValuesS = noisySeeedsS.run();
	steady_clock::time_point endS = steady_clock::now();

	steady_clock::time_point beginP = steady_clock::now();
	alg::NoisySeedsParallel noisySeedsP(graph1, graph2, threshold, set);
	auto matchedValuesP = noisySeedsP.run();
	steady_clock::time_point endP = steady_clock::now();

	
	auto sAccuracy = matchedValuesS->getAccuracy();
	auto pAccuracy = matchedValuesP->getAccuracy();

	gh::MatchedPairsResults results(threshold, (int)set->getNodeSets().size(), graph1->getNodesCount(), graph1->getEdgesCount(), graph2->getEdgesCount(), 
									duration_cast<milliseconds>(endS - beginS).count(),	duration_cast<milliseconds>(endP - beginP).count(),sAccuracy,pAccuracy);
	gh::saveMatchedResultsToFile(results);

    delete matchedValuesS;
	delete matchedValuesP;
	delete set;
	delete graph1;
	delete graph2;
}

void compareExpandOnceImplementations() {
	Graph* graph1 = gh::createGraphFromFile(FilesConfig::getGraphFileName(csr::graph1), FilesConfig::LINES_TO_SKIP);
	Graph* graph2 = gh::createGraphFromFile(FilesConfig::getGraphFileName(csr::graph2), FilesConfig::LINES_TO_SKIP);
	MatchedPairsSet* set = gh::createSeedSetFromFile(FilesConfig::getNoisySeedSetName());

	std::chrono::steady_clock::time_point beginS = std::chrono::steady_clock::now();
	alg::ExpandOnceSerial expandOnceS(graph1, graph2, threshold, seedSize, set);
	auto matchedValuesS = expandOnceS.run();
	std::chrono::steady_clock::time_point endS = std::chrono::steady_clock::now();


	std::chrono::steady_clock::time_point beginP = std::chrono::steady_clock::now();
	alg::ExpandOnceParallel expandOnceP(graph1, graph2, threshold, seedSize, set);
	//auto matchedValuesP = expandOnceP.run();
	std::chrono::steady_clock::time_point endP = std::chrono::steady_clock::now();


	cout << "SERIAL: ";
	matchedValuesS->printAccuracy();
	cout << "PARALLEL: ";
	//matchedValuesP->printAccuracy();

	std::cout << "SERIAL  : " << std::chrono::duration_cast<std::chrono::milliseconds> (endS - beginS).count() << "[ms]" << std::endl;
	std::cout << "PARALLEL: " << std::chrono::duration_cast<std::chrono::milliseconds> (endP - beginP).count() << "[ms]" << std::endl;


	delete matchedValuesS;
	//delete matchedValuesP;
	delete set;
	delete graph1;
	delete graph2;
}




int main(int argc, char* argv[]) {
	parseArguements(argc, argv);

	compareNoisySeedsImplementations();
	//compareExpandOnceImplementations();
	
}

