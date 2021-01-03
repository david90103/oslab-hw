#ifndef TSP_DE1D_H_
#define TSP_DE1D_H_

#include "../lib/lib1d.hpp"

class DE1D : public TSPAlgorithm1D {
    private:
        double f;
        double crossover_rate;
        vector<vector<double>> mutation(vector<vector<double>> population);
        vector<vector<double>> crossover(vector<vector<double>> population, vector<vector<double>> v_arr);
    public:
        DE1D(unsigned int randseed, int population_size, double crossover_rate, double f, char const *seedfile);
        vector<double> run(int generations);
};

#endif // TSP_DE1D_H_