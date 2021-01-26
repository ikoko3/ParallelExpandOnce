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


int threshold = 6;
int seedSize = 10;

using namespace csr;


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

void printResultsToFile() {
	ofstream myfile;
	myfile.open("C:\\Users\\john_\\Documents\\tbb_env\\res.json");
	myfile << "{";
	myfile << "\"threshold\":"<< threshold <<",";
	myfile << "\"Nodes\":"<< threshold <<",";
	myfile << "\"G1Edges\":"<< threshold <<",";
	myfile << "\"G2Edges\":"<< threshold <<",";
	myfile << "\"Stime\":1.55,\"Saccuracy\":0.67,";
	myfile << "\"Ptime\":1.55,\"Paccuracy\":0.67";
	myfile << "}";
	myfile.close();
}

void compareNoisySeedsImplementations() {
	Graph* graph1 = gh::createGraphFromFile(GraphFilesConfig::getGraphFileName(csr::graph1), GraphFilesConfig::LINES_TO_SKIP);
	Graph* graph2 = gh::createGraphFromFile(GraphFilesConfig::getGraphFileName(csr::graph2), GraphFilesConfig::LINES_TO_SKIP);
	MatchedPairsSet* set = gh::createSeedSetFromFile(GraphFilesConfig::getNoisySeedSetName());

	std::chrono::steady_clock::time_point beginS = std::chrono::steady_clock::now();
	alg::NoisySeedsSerial noisySeeedsS(graph1, graph2, threshold, set);
	auto matchedValuesS = noisySeeedsS.run();
	std::chrono::steady_clock::time_point endS = std::chrono::steady_clock::now();

	std::chrono::steady_clock::time_point beginP = std::chrono::steady_clock::now();
	alg::NoisySeedsParallel noisySeedsP(graph1, graph2, threshold, set);
	auto matchedValuesP = noisySeedsP.run();
	std::chrono::steady_clock::time_point endP = std::chrono::steady_clock::now();
	
	long sTime = chrono::duration_cast<std::chrono::milliseconds>(endS - beginS).count();
	long pTime = chrono::duration_cast<std::chrono::milliseconds>(endP - beginP).count();
	auto sAccuracy = matchedValuesS->getAccuracy();
	auto pAccuracy = matchedValuesP->getAccuracy();

	//cout << "SERIAL: ";
	//matchedValuesS->printAccuracy();
	//cout << "PARALLEL: ";
	//matchedValuesP->printAccuracy();

	//std::cout << "SERIAL  : " << sTime << "[ms]" << std::endl;
	//std::cout << "PARALLEL: " << pTime << "[ms]" << std::endl;

	ofstream resultsFile;
	resultsFile.open("C:\\Users\\john_\\Documents\\tbb_env\\res.json");
	resultsFile << "{";
	resultsFile << "\"threshold\":" << threshold << ",";
	resultsFile << "\"Nodes\":" << graph1->getNodesCount() << ",";
	resultsFile << "\"G1Edges\":" << graph1->getEdgesCount() << ",";
	resultsFile << "\"G2Edges\":" << graph2->getEdgesCount()<< ",";
	resultsFile << "\"Stime\":"<< sTime <<",\"Saccuracy\":"<< sAccuracy.accuraccy<<",";
	resultsFile << "\"Scorrect\":"<< sAccuracy.correct <<",\"Stotal\":"<<sAccuracy.total<<",";
	resultsFile << "\"Ptime\":"<< pTime <<",\"Paccuracy\":"<< pAccuracy.accuraccy << ",";
	resultsFile << "\"Pcorrect\":" << pAccuracy.correct << ",\"Ptotal\":" << pAccuracy.total;
	resultsFile << "}";
	resultsFile.close();


    delete matchedValuesS;
	delete matchedValuesP;
	delete set;
	delete graph1;
	delete graph2;
}

void compareExpandOnceImplementations() {
	Graph* graph1 = gh::createGraphFromFile(GraphFilesConfig::getGraphFileName(csr::graph1), GraphFilesConfig::LINES_TO_SKIP);
	Graph* graph2 = gh::createGraphFromFile(GraphFilesConfig::getGraphFileName(csr::graph2), GraphFilesConfig::LINES_TO_SKIP);
	MatchedPairsSet* set = gh::createSeedSetFromFile(GraphFilesConfig::getNoisySeedSetName());

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

