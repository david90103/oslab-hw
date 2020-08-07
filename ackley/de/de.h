#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/**
 * Differential Evolution
 */
class DE {
    private:
        double bestScore;
        int population_size;
        double crossover_rate;
        double f;
        int dimension;
        const double INTERVAL_RANGE = 30;
        vector<vector<double>> population;
        vector<double> objective_values;
        vector<double> global_best;
        vector<double> result;
        double evaluate(vector<double> target);
        vector<vector<double>> mutation(vector<vector<double>> population);
        vector<vector<double>> crossover(vector<vector<double>> population, vector<vector<double>> arr);

    public:
        DE(unsigned int randseed, int population_size, int dimension, double crossover_rate, double f, char const *seedfile);
        double getBestScore();
        vector<double> run(int iterations);
};