#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class My {
    private:
        double bestScore;
        vector<vector<double>> population;
        vector<vector<double>> cities;
        vector<vector<double>> distances;
        int population_size;
        int dimension;
        double f;
        double crossover_rate;
        vector<double> fitness_values;
        vector<double> global_best;
        vector<int> best;
        vector<double> result;
        int findNearest(vector<double> position, vector<int> candidate_cities);
        double distance(vector<double> city_a, vector<double> city_b);
        double evaluate(vector<int> path);
        double evaluateWithDistance(vector<double> coordinates, vector<int> path);
        vector<int> convertToPath(vector<double> coordinates);
        vector<vector<double>> mutation(vector<vector<double>> population);
        vector<vector<double>> crossover(vector<vector<double>> population, vector<vector<double>> v_arr);
    public:
        My(unsigned int randseed, int population_size, double crossover_rate, double f, char const *seedfile);
        double getBestScore();
        vector<double> run(int generations);
};