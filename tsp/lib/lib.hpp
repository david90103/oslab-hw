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

/**
 * TSPAlgorithm class
 * An abstract class for solving tsp problem using contiguous optimization.
 */
class TSPAlgorithm {
protected:
    double bestScore;
    int dimension;
    int population_size;
    vector<vector<double>> population;
    vector<vector<double>> cities;
    vector<vector<double>> distances;
    vector<double> fitness_values;
    vector<double> global_best;
    vector<int> best;
    vector<double> result;
    double evaluate(vector<int> path);
    double evaluateWithDistance(vector<double> coordinates, vector<int> path);
    vector<vector<double>> readCitiesFromFile(char const *seedfile);
    vector<int> convertToPath(vector<double> coordinates);
    double distance(vector<double> city_a, vector<double> city_b);
    int findNearest(vector<double> position, vector<int> candidate_cities);
public:
    virtual vector<double> run(int generations) = 0;
};