#ifndef TSP_DE1DC_H_
#define TSP_DE1DC_H_

#include "../lib/lib1dc.hpp"
#include <algorithm>

class DE1DC : public TSPAlgorithm1DC {
    private:
        double f;
        double crossover_rate;
        vector<vector<double>> mutation(vector<vector<double>> population);
        vector<vector<double>> crossover(vector<vector<double>> population, vector<vector<double>> v_arr);
    public:
        DE1DC(unsigned int randseed, int population_size, double crossover_rate, double f, char const *seedfile);
        vector<double> run(int generations);
};

#endif // TSP_DE1DC_H_