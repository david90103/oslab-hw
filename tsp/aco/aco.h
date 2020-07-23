#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
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
        double fitness_values_sum;
        vector<double> fitness_values;
        vector<int> best;
        vector<double> result;
        double distance(vector<double> city_a, vector<double> city_b);
        int fitness(vector<bool> arr);
        int rouletteWheel(int current_city, vector<int> candidate_city);
        void evalPopulation(vector<bool> is_new_member);
        void generatePath();
        void updatePheromone();
    public:
        ACO(int ants, double a, double b, double r, double q, char const *seedfile);
        void printArray();
        double getBestScore();
        vector<int> run(int generations);
};