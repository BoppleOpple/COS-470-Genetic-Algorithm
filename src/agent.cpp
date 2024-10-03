#include "agent.h"
#include <climits>
#include <cstdlib>
#include <iostream>
#include <string>

Agent::Agent() {
	fitness = 0.0;
}

Agent::Agent(int numCharacters) {
	fitness = 0.0;
	for (int i = 0; i < numCharacters; i++) {
		genes.push_back((char)(std::rand()%CHAR_MAX));
	}
}

bool Agent::operator<(const Agent &other) {
	return fitness < other.fitness;
}
bool Agent::operator>(const Agent &other) {
	return fitness > other.fitness;
}

/*
mutation:
character 00110101
mask      00001001
result    00111100

( character OR mask ) - ( character AND mask )
          00110101
       OR 00001001
          00111101
        - 00000001
result    00111100
*/

/**/
Agent *Agent::crossover(const Agent *o, const float &mr) {
	int breakpoint = std::rand() % genes.length();

	std::string newGenes = genes.substr(0, breakpoint) + o->genes.substr(breakpoint);

	for (int i = 0; i < newGenes.length(); i++) {
		/*
		// mutate binary
		char mutationMask = 0;

		for (int j = 0; j < CHAR_BIT; j++) {
			if (rand() < RAND_MAX * mr) 
				mutationMask++;

			mutationMask <<= 1;
		}

		// if (mutationMask) printf("mutating with mask: %hhX\n", mutationMask);

		newGenes[i] = ( newGenes[i] | mutationMask ) - ( newGenes[i] & mutationMask );
		*/

		// mutate chars
		if (rand() < RAND_MAX * mr) 
			newGenes[i] = rand() % CHAR_MAX;
	}

	Agent *child = new Agent(newGenes.length());

	child->setGenes(newGenes);

	return child;
}

double Agent::calcFitness(const std::string &target) {
	fitness = 0.0;

	for (int i = 0; i < genes.length(); i++) {
		// MSQE sort of thing, just a function with a maximum at the desired value
		// fitness += 1 - double(abs( genes[i] - target[i] )) / double(CHAR_MAX);

		// just count how many are right
		if (genes[i] == target[i]) fitness++;
	}

	return fitness;
}

double Agent::getFitness() {
	return fitness;
}

std::string Agent::getGenes() {
	return genes;
}

void Agent::setGenes(std::string goal) {
	genes = goal;
}

void Agent::print() {
	std::cout << "|----------------------------------------------------------|" << std::endl;
	std::cout << "Agent Genes: \"" << genes << "\"" << std::endl << std::endl;
	std::cout << "Agent Fitness: " << fitness << "" << std::endl;
	std::cout << "|----------------------------------------------------------|" << std::endl;
}