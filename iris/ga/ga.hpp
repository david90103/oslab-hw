#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/**
 * Genetic Algorithm
 */
class GA {
    private:
        int clusters;
        int encode_type;
        int population_size;
        int dimension;
        double bestScore;
        double crossover_rate;
        double mutation_rate;
        vector<vector<double>> iris;
        vector<vector<int>> population;
        double fitness_values_sum;
        vector<double> fitness_values;
        vector<int> best;
        vector<double> result;
        double fitness(vector<int> arr);
        void evalPopulation(vector<bool> is_new_member);
        vector<int> rouletteWheel();
        vector<int> tournament();
        vector<vector<int>> crossover(vector<int> father, vector<int> mother);
        inline vector<int> mutation(vector<int> target);
    public:
        GA(time_t randseed, int clusters, int encode_type, int population_s, double crossover_r, double mutation_r, char const *seedfile);
        void printArray();
        double getBestScore();
        vector<double> run(int generations);
};