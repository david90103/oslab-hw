#ifndef TSP_GAP_H_
#define TSP_GAP_H_

#include <thread>
#include <mutex>
#include "ga.hpp"

class GAP : public GA {
private:
    // Pass a solution to another population every 100 iterations
    const int EXCHANGE_SOLUTION_GEN = 100;
    int total_threads;
    // Shared in all threads
    static int ready_to_exchange;
    static vector<vector<int>> exchange_pool;
    // This is a mutex for ready_to_exchange and exchange_pool
    static mutex exchange_lock;
    static int finished;
    static mutex finished_lock;
    static mutex best_score_lock;
    // Override evalPopulation because we need to lock bestScore
    vector<double> evalPopulation(vector<bool> is_new_member, vector<vector<int>> pop, vector<double> f_values);
    vector<vector<int>> exchange(vector<vector<int>> population);
    void threadHandler(int generations);
public:
    GAP(): GA() {};
    GAP(
        int bits, 
        int populations, 
        int population_s, 
        double crossover_r, 
        double mutation_r, 
        char const *crossover_method, 
        char const *seedfile
    );
    vector<double> run(int generations);
};

#endif // TSP_GAP_H_