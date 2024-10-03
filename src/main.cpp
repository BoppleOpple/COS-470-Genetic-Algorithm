#include <__config>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cfloat>
#include <functional>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "agent.h"

const float MUTATION_RATE = 5e-03;
// const int PARENT_COUNT = 2; // ONLY 2 FOR NOW
const int POPULATION_SIZE = 2000;
const int POOL_SIZE = POPULATION_SIZE / 2;
const int GENERATIONS = 1000;

bool fitnessCompare(Agent *a, Agent *b) {
	return *a > *b;
}

Agent *selectParent(const std::vector<Agent*> &pop, double maxFitnessIndex = 0.0) {
	if (maxFitnessIndex == 0.0) {
		for (int i = 0; i < POOL_SIZE; i++)
			maxFitnessIndex += pop[i]->getFitness();
	}

	double fitnessIndex = maxFitnessIndex * double(rand()) / double(RAND_MAX);

	for (int i = 0; i < pop.size(); i++) {
		fitnessIndex -= pop[i]->getFitness();
		if (fitnessIndex <= 0) return pop[i];
	}
	return nullptr;
}

int main(int argc, char *argv[]){
	std::ifstream targetTextFile;
	std::string targetText;
	std::string line;
	std::string filename;
	std::vector<Agent*> population;
	std::vector<Agent*> newPopulation;
	double maxFitnessSelection = 0.0;

	std::srand(std::time(nullptr));

	if (argc < 2) {
		std::cout << "What file to read from?" << std::endl;
		std::cin >> filename;
	} else {
		filename = argv[1];
	}

	targetTextFile.open(filename);

	if (targetTextFile.is_open()) {
		while (std::getline(targetTextFile, line)) {
			targetText.append(line + "\n");
		}
		// remove extra newline
		targetText.erase(targetText.length()-1, 1);
	}

	for (int i = 0; i < POPULATION_SIZE; i++) {
		population.push_back(new Agent(targetText.length()));
		population[i]->calcFitness(targetText);
	}

	for (int i = 0; i < GENERATIONS; i++) {
		maxFitnessSelection = 0.0;
		newPopulation.clear();

		for (int i = 0; i < POPULATION_SIZE; i++)
			population[i]->calcFitness(targetText);
		std::sort(population.begin(), population.end(), fitnessCompare);

		std::cout << "best of generation " << i << ": " << population[0]->getFitness() << std::endl;

		if (population[0]->getGenes() == targetText){
			std::cout << "match found! exiting." << std::endl;
			population[0]->print();
			return 0;
		}
		
		for (int i = 0; i < POOL_SIZE; i++)
			maxFitnessSelection += population[i]->getFitness();
		
		while (newPopulation.size() < POPULATION_SIZE) {
			Agent *p1 = selectParent(population, maxFitnessSelection);
			Agent *p2 = selectParent(population, maxFitnessSelection);

			newPopulation.push_back(p1->crossover(p2, MUTATION_RATE));
		}
		
		for (int i = 0; i < POPULATION_SIZE; i++){
			population[i] = newPopulation[i];
		}
	}
	return 0;
}