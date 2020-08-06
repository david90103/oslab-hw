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
 * Particle Swarm Optimization 
 */
class PSO {
    private:
        double bestScore;
        int population_size;
        double w;
        double c1;
        double c2;
        int dimension;
        const double INTERVAL_RANGE = 30;
        vector<vector<double>> population;
        vector<vector<double>> velocities;
        vector<double> objective_values;
        vector<double> individual_bests;
        vector<vector<double>> individual_bests_pos;
        vector<double> global_best;
        vector<double> result;
        double evaluate(vector<double> target);
        void evaluatePopulation();
        void updateVelocity();
        void updatePosition();
    public:
        PSO(unsigned int randseed, int dimension, int population_size, double w, double c1, double c2, char const *seedfile);
        double getBestScore();
        vector<double> run(int iterations);
};