#include "agent.h"
#include <climits>
#include <cstdlib>
#include <iostream>
#include <string>

/*
 * Only init fitness to 0
**/
Agent::Agent() {
	fitness = 0.0;
}

/*
 * Init fitness to zero and add random genes
 * @param numCharacters The number of random characters to generate for genes
**/
Agent::Agent(int numCharacters) {
	fitness = 0.0;
	for (int i = 0; i < numCharacters; i++) {
		genes.push_back((char)(std::rand()%CHAR_MAX));
	}
}

//basic operator overrides
bool Agent::operator<(const Agent &other) {
	return fitness < other.fitness;
}

bool Agent::operator>(const Agent &other) {
	return fitness > other.fitness;
}

/*
my justification for the bitmasking neded for single-bit mutation:

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

/*
 * Returns a child agent with genes from this agent and another parent
 * @param o Other parent
 * @param mr A reference to the global mutation rate
**/
Agent *Agent::crossover(const Agent *o, const float &mr) {
	// select a point to split the genes between parents
	int breakpoint = std::rand() % genes.length();

	// make the child's genes
	std::string newGenes = genes.substr(0, breakpoint) + o->genes.substr(breakpoint);

	// mutate them somehow
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

		// mutate chars instead, it works way better
		if (rand() < RAND_MAX * mr) 
			newGenes[i] = rand() % CHAR_MAX;
	}

	// make a new child with the new genes
	Agent *child = new Agent(newGenes.length());
	child->setGenes(newGenes);

	// return it
	return child;
}

/*
 * Calculates the fitness based on some expected value, and both sets it for this
 * actor and returns the new fitness
 * @param target A reference to the correct genes
**/
double Agent::calcFitness(const std::string &target) {
	fitness = 0.0;

	for (int i = 0; i < genes.length(); i++) {
		// MSQE sort of thing, just a function with a maximum at the desired value
		// fitness += 1 - double(abs( genes[i] - target[i] )) / double(CHAR_MAX);

		// just count how many are right, it works better and isnt a pain
		if (genes[i] == target[i]) fitness++;
	}

	return fitness;
}

/*
 * Returns this agent's fitness
**/
double Agent::getFitness() {
	return fitness;
}

/*
 * Returns this agent's genes
**/
std::string Agent::getGenes() {
	return genes;
}

/*
 * Sets this agent's fitness
 * @param goal The agent's new genes 
**/
void Agent::setGenes(std::string goal) {
	genes = goal;
}

/*
 * Prints this agent's genes and fitness to stdout
**/
void Agent::print() {
	std::cout << "|----------------------------------------------------------|" << std::endl;
	std::cout << "Agent Genes: \"" << genes << "\"" << std::endl << std::endl;
	std::cout << "Agent Fitness: " << fitness << "" << std::endl;
	std::cout << "|----------------------------------------------------------|" << std::endl;
}