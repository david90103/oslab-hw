#include "../lib/lib.hpp"

class PSO : TSPAlgorithm {
    private:
        double w;
        double c1;
        double c2;
        vector<vector<double>> cities_normalized;
        vector<vector<double>> distances_normalized;
        int animation_file_count;
        vector<vector<double>> velocities;
        vector<double> individual_bests;
        vector<double> objective_values;
        vector<vector<double>> individual_bests_pos;
        double bestFitness;
        vector<vector<double>> normalize(vector<vector<double>> cities);
        void evaluatePopulation();
        void updateVelocity();
        void updatePosition();
    public:
        PSO(unsigned int randseed, int population_size, double w, double c1, double c2, char const *seedfile);
        vector<double> run(int generations);
};