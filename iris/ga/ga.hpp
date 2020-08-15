#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <climits>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Chromosome {
    vector<int> cluster_id_encoded;
    vector<vector<double>> centroids_encoded;
};

/**
 * Abstract class Genetic Algorithm
 */
class GA {
    protected:
        int clusters;
        int encode_type;
        int population_size;
        int dimension;
        double bestScore;
        double crossover_rate;
        double mutation_rate;
        vector<Chromosome> population;
        vector<vector<double>> iris;
        vector<vector<double>> iris_normalized;
        Chromosome best;
        double fitness_values_sum;
        vector<double> fitness_values;
        vector<double> result;
        void evalPopulation(vector<bool> is_new_member);
        Chromosome rouletteWheel();
        Chromosome tournament();
        double fitness_cluster_id(vector<int> arr);
        virtual void initPopulation() = 0;
        virtual double fitness(Chromosome arr) = 0;
        virtual vector<Chromosome> crossover(Chromosome father, Chromosome mother) = 0;
        virtual Chromosome mutation(Chromosome) = 0;
    public:
        GA(time_t randseed, int clusters, int encode_type, int population_s, double crossover_r, double mutation_r, char const *seedfile);
        virtual void printBestChromosome() = 0;
        double getBestScore();
        vector<double> run(int generations);
};

/**
 * Encode with cluster id
 */
class ClusterIdGA : public GA {
    protected:
        // vector<vector<int>> population;
        void initPopulation();
        double fitness(Chromosome arr);
        vector<Chromosome> crossover(Chromosome father, Chromosome mother);
        Chromosome mutation(Chromosome);
    public:
        ClusterIdGA(time_t randseed, int clusters, int encode_type, int population_s, double crossover_r, double mutation_r, char const *seedfile)
            : GA(randseed, clusters, encode_type, population_s, crossover_r, mutation_r, seedfile) {};
        void printBestChromosome();
};

/**
 * Encode with centroids position
 */
class CentroidsGA : public GA {
    protected:
        // vector<vector<vector<double>>> population;
        void initPopulation();
        inline double distance(vector<double> point, vector<double> centroid);
        Chromosome convertToClusterId(Chromosome);
        double fitness(Chromosome arr);
        vector<Chromosome> crossover(Chromosome father, Chromosome mother);
        Chromosome mutation(Chromosome);
    public:
        CentroidsGA(time_t randseed, int clusters, int encode_type, int population_s, double crossover_r, double mutation_r, char const *seedfile)
            : GA(randseed, clusters, encode_type, population_s, crossover_r, mutation_r, seedfile) {};
        void printBestChromosome();
};
