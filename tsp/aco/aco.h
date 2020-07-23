#include <stdlib.h>
#include <time.h>
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
        int size;
        int bestScore;
        vector<vector<bool>> population;
        vector<vector<double>> cities;
        vector<vector<double>> pheromone;
        vector<vector<double>> distance;
        int ants;
        double alpha;
        double beta;
        double rho;
        double q;
        int fitness_values_sum;
        vector<int> fitness_values;
        vector<bool> best;
        vector<int> result;
        int fitness(vector<bool> arr);
        void evalPopulation(vector<bool> is_new_member);
    public:
        ACO(int bits, int ants, double a, double b, double r, double q, char const *seedfile);
        void printArray();
        int getBestScore();
        vector<int> run(int generations);
};