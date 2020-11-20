#ifndef TSP_GA_H_
#define TSP_GA_H_

#include <vector>
#include <string>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <float.h>
#include <math.h>
using namespace std;

class GA {
private:
    double bestScore;
    int population_size;
    double crossover_rate;
    double mutation_rate;
    vector<vector<double>> cities;
    vector<vector<double>> distances;
    vector<vector<int>> population;
    int fitness_values_sum;
    vector<double> fitness_values;
    vector<int> best;
    vector<double> result;
    vector<vector<double>> readCitiesFromFile(char const *seedfile);
    vector<vector<double>> initDistances(vector<vector<double>> cities);
    vector<vector<int>> initPopulation(vector<vector<double>> cities);
    double distance(vector<double> a, vector<double> b);
    int fitness(vector<int> arr);
    void evalPopulation(vector<int> is_new_member);
    vector<int> rouletteWheel();
    vector<int> tournament();
    vector<vector<int>> (GA::*crossover)(vector<int>, vector<int>);
    vector<vector<int>> partiallyMappedCrossover(vector<int> father, vector<int> mother);
    vector<vector<int>> cycleCrossover(vector<int> father, vector<int> mother);
    vector<vector<int>> orderCrossover(vector<int> father, vector<int> mother);
    inline vector<int> mutation(vector<int> target);
public:
    GA(int bits, int population_s, double crossover_r, double mutation_r, char const *crossover_method, char const *seedfile);
    vector<double> run(int generations);
};

#endif // TSP_GA_H_