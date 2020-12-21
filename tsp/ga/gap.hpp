#ifndef TSP_GAP_H_
#define TSP_GAP_H_

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
#include "ga.hpp"
using namespace std;

class GAP : public GA {
public:
    GAP() : GA() {}
    GAP(int bits, int population_s, double crossover_r, double mutation_r, char const *crossover_method, char const *seedfile);
    vector<double> run(int generations);
};

#endif // TSP_GAP_H_