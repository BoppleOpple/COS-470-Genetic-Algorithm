#include <string>

class Agent {
	private:
		std::string genes;
		double fitness;
	public:
		Agent();
		Agent(int numCharacters);


		// operator overriding for sorting (and convenience)
		bool operator<(const Agent &other);
		bool operator>(const Agent &other);

		// getters and setters, no setter needed for fitness
		double getFitness();
		std::string getGenes();
		void setGenes(std::string goal);
		
		// actual useful functions
		Agent *crossover(const Agent *o, const float &mr);
		double calcFitness(const std::string &target);
		void print();
};