#include "../lib/lib.hpp"

class DE : TSPAlgorithm {
    private:
        double f;
        double crossover_rate;
        vector<vector<double>> mutation(vector<vector<double>> population);
        vector<vector<double>> crossover(vector<vector<double>> population, vector<vector<double>> v_arr);
    public:
        DE(unsigned int randseed, int population_size, double crossover_rate, double f, char const *seedfile);
        vector<double> run(int generations);
};