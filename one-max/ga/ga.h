#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

/**
 * Genetic Algorithm
 */
class GA {
    private:
        int size;
        int bestScore;
        int population_size;
        double crossover_rate;
        double mutation_rate;
        vector<vector<bool>> population;
        vector<int> fitness_values;
        vector<bool> best;
        vector<int> result;
        int fitness(vector<bool> arr);
        void evalPopulation();
        vector<bool> rouletteWheel();
        vector<bool> tournament();
        vector<vector<bool>> crossover(vector<bool> father, vector<bool> mother);
        inline vector<bool> mutation(vector<bool> target);
    public:
        GA(int bits, int population_s, double crossover_r, double mutation_r, char const *seedfile);
        void printArray();
        int getBestScore();
        vector<int> run(int generations);
};