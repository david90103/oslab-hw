#ifndef TSP_GAP_H_
#define TSP_GAP_H_

#include <thread>
#include <mutex>
#include "ga.hpp"

class GAP : public GA {
private:
    // Pass a solution to another population every EXCHANGE_SOLUTION_GEN iterations
    static const int EXCHANGE_SOLUTION_GEN = 100;
    int total_threads;
    // Shared in all threads
    static int ready_to_exchange;
    static int current_exchange_gen;
    static int threads_to_wait;
    static vector<vector<int>> exchange_pool;
    // Lock for ready_to_exchange and exchange_pool
    static mutex exchange_lock;
    // Lock for bestScore and best
    static mutex best_score_lock;
    // Lock for result vector
    static mutex result_lock;
    // Override evalPopulation because we need to lock bestScore
    vector<double> evalPopulation(vector<bool> is_new_member, vector<vector<int>> pop, vector<double> f_values);
    vector<vector<int>> exchange(vector<vector<int>> population, int generation);
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