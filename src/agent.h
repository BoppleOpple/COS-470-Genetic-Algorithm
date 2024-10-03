#include <string>

class Agent {
	private:
		std::string genes;
		double fitness;
	public:
		Agent();
		Agent(int numCharacters);

		bool operator<(const Agent &other);
		bool operator>(const Agent &other);

		double getFitness();
		std::string getGenes();
		void setGenes(std::string goal);

		Agent *crossover(const Agent *o, const float &mr);
		double calcFitness(const std::string &target);
		void print();
};