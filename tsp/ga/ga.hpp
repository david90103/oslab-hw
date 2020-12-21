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
#include <algorithm>
#include <float.h>
#include <math.h>
using namespace std;

class GA {
protected:
    double bestScore;
    int population_size;
    double crossover_rate;
    double mutation_rate;
    vector<vector<double>> cities;
    vector<vector<double>> distances;
    vector<vector<int>> population;
    vector<double> fitness_values;
    vector<int> best;
    vector<double> result;
    vector<vector<double>> readCitiesFromFile(char const *seedfile);
    vector<vector<double>> initDistances(vector<vector<double>> cities);
    vector<vector<int>> initPopulation(vector<vector<double>> cities);
    double distance(vector<double> a, vector<double> b);
    int fitness(vector<int> arr);
    vector<double> evalPopulation(vector<bool> is_new_member, vector<vector<int>> pop, vector<double> f_values);
    bool isValidPath(vector<int> path);
    void printPath(vector<int> path);
    vector<int> rouletteWheel();
    vector<int> tournament();
    vector<int> rouletteWheel(vector<vector<int>> pop, vector<double> f_value);
    vector<int> tournament(vector<vector<int>> pop, vector<double> f_value);
    vector<vector<int>> (GA::*crossover)(vector<int>, vector<int>);
    vector<int> mutation(vector<int> target);
public:
    GA();
    GA(int bits, int population_s, double crossover_r, double mutation_r, char const *crossover_method, char const *seedfile);
    vector<double> run(int generations);
    // For GAP class to use
    vector<vector<int>> partiallyMappedCrossover(vector<int> father, vector<int> mother);
    vector<vector<int>> cycleCrossover(vector<int> father, vector<int> mother);
    vector<vector<int>> orderCrossover(vector<int> father, vector<int> mother);
};

#endif // TSP_GA_H_