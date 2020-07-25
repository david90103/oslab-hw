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
 * Ant Colony Optimization
 */
class ACO {
    private:
        double bestScore;
        vector<vector<int>> population;
        vector<vector<double>> cities;
        vector<vector<double>> pheromone;
        vector<vector<double>> distances;
        int ants;
        double alpha;
        double beta;
        double rho;
        double q;
        vector<double> fitness_values;
        vector<int> best;
        vector<double> result;
        double distance(vector<double> city_a, vector<double> city_b);
        int rouletteWheelwithRBP(int current_city, vector<int> candidate_city);
        double evaluate(vector<int> path);
        void generatePath();
        void updatePheromone();
    public:
        ACO(unsigned int randseed, int ants, double a, double b, double r, double q, char const *seedfile);
        // void printArray();
        double getBestScore();
        vector<double> run(int generations);
};