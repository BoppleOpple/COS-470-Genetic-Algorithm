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
const int POOL_SIZE = POPULATION_SIZE / 2; // the number of agents that could be parents
const int GENERATIONS = 1000;

/*
 * Comparison function for sorting a population of Agent pointers
**/
bool fitnessCompare(Agent *a, Agent *b) {
	return *a > *b;
}

/*
 * Given a population (and perhaps a maximum fitness selection to save time)
 * selects two parents randomly, weighted by fitness.
 *
 * @param pop A reference to a population of Agent pointers
 * @param maxFitnessIndex The maximum cumulative fitness that can be randomly
 *   selected. Acts as a threshold for how fit parents must be relative to the
 *   rest of the population.
**/
Agent *selectParent(const std::vector<Agent*> &pop, double maxFitnessIndex = 0.0) {
	// if no index is given, calculate it
	if (maxFitnessIndex == 0.0) {
		// the maximum fitness is just the sum of the fitnesses of the top n Agents
		for (int i = 0; i < POOL_SIZE; i++)
			maxFitnessIndex += pop[i]->getFitness();
	}

	// select a random fitness from the range
	double fitnessIndex = maxFitnessIndex * double(rand()) / double(RAND_MAX);

	// find the corresponding agent. the agent fitness acts like a weight on 
	// the randomly generated number, so more fit individuals are (slightly)
	// more likely to reproduce
	for (int i = 0; i < pop.size(); i++) {
		fitnessIndex -= pop[i]->getFitness();
		if (fitnessIndex <= 0) return pop[i];
	}

	// if nothing was found, something has gone horribly wrong
	return nullptr;
}

int main(int argc, char *argv[]){
	// declare all the variables we'll be using later
	std::ifstream targetTextFile;
	std::string targetText;
	std::string line;
	std::string filename;
	std::vector<Agent*> population;
	std::vector<Agent*> newPopulation;
	double maxFitnessSelection = 0.0;

	// randomly seed rng
	std::srand(std::time(nullptr));

	// if a file isnt given through arguments, ask for one
	if (argc < 2) {
		std::cout << "What file to read from?" << std::endl;
		std::cin >> filename;
	} else {
		// otherwise use the passed file
		filename = argv[1];
	}

	// try to read the file
	targetTextFile.open(filename);
	if (targetTextFile.is_open()) {
		// add each line to a string
		while (std::getline(targetTextFile, line)) {
			targetText.append(line + "\n");
		}
		// remove extra newline
		targetText.erase(targetText.length()-1, 1);
	} else {
		// throw a fit if we can't
		std::cout << "Couldn't read file \"" << filename << "\"" << std::endl;
		return 1;
	}

	// iniialize the population
	for (int i = 0; i < POPULATION_SIZE; i++) {
		population.push_back(new Agent(targetText.length()));

		// calc fitness (for debugging)
		// population[i]->calcFitness(targetText);
	}

	for (int i = 0; i < GENERATIONS; i++) {
		// reset variables
		maxFitnessSelection = 0.0;
		newPopulation.clear();

		// calculate and sort by fitness for all agents
		for (int i = 0; i < POPULATION_SIZE; i++)
			population[i]->calcFitness(targetText);
		std::sort(population.begin(), population.end(), fitnessCompare);

		// print the best one
		std::cout << "best of generation " << i << ": " << population[0]->getFitness() << std::endl;

		// exit if there is a match
		if (population[0]->getGenes() == targetText){
			std::cout << "match found! exiting." << std::endl;
			population[0]->print();
			return 0;
		}
		
		// precompute the maximum fitness for selection (see selectParent)
		for (int i = 0; i < POOL_SIZE; i++)
			maxFitnessSelection += population[i]->getFitness();
		
		// repopulate the new population
		while (newPopulation.size() < POPULATION_SIZE) {
			Agent *p1 = selectParent(population, maxFitnessSelection);
			Agent *p2 = selectParent(population, maxFitnessSelection);

			newPopulation.push_back(p1->crossover(p2, MUTATION_RATE));
		}
		
		// copy it over, probably slowly
		for (int i = 0; i < POPULATION_SIZE; i++){
			population[i] = newPopulation[i];
		}
	}
	// if its never found, break and act like nothing happened
	return 0;
}