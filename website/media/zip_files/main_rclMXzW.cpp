#include "algorithm.hpp"
#include "ackley.hpp"
#include "Individual.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>


int main() {
	const unsigned DIMENSION = 4;
	const unsigned NUMBER_OF_INDIVIDUALS = 50;
	const unsigned NUMBER_OF_GENERATIONS = 2000;
	const unsigned NUMBER_OF_REPETITIONS = 51;
	int selOption = 0;

	std::cout << "Selection option: " << std::endl <<"1 -> Roulette Selection\n2 -> Tournament Selection\n3 -> bestFraction Selection" << std::endl;
	std::cin >> selOption;

	std::ofstream outfile;
	std::ofstream outfileDetailed;
	outfile.open("data.txt", std::ofstream::out | std::ofstream::trunc);
	outfileDetailed.open("details.txt", std::ofstream::out | std::ofstream::trunc);
	
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < NUMBER_OF_REPETITIONS; ++i) {
		Algorithm geneAlgorithm(DIMENSION, NUMBER_OF_INDIVIDUALS, NUMBER_OF_GENERATIONS, selOption);
		std::vector<Individual> Population = geneAlgorithm.runAlgorithm(outfileDetailed, i);
		outfile << geneAlgorithm.getBestFit(Population) << " ";
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	outfile.close();
	outfileDetailed.close();
    return 0;
}