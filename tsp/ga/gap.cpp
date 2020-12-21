#include "gap.hpp"

// Reference: https://coherence0815.wordpress.com/2015/04/19/static-class-member-initialization-in-c/
int GAP::ready_to_exchange = 0;
int GAP::finished = 0;
mutex GAP::exchange_lock;
mutex GAP::finished_lock;
mutex GAP::best_score_lock;
vector<vector<int>> GAP::exchange_pool = vector<vector<int>>();

GAP::GAP(int bits,int populations, int population_s, double crossover_r, double mutation_r, char const *crossover_method, char const *seedfile) {
    // GA(bits, population_s, crossover_r, mutation_r, crossover_method, seedfile);
    srand(time(NULL));
    bestScore = DBL_MAX;
    population_size = population_s;
    crossover_rate = crossover_r;
    mutation_rate = mutation_r;
    fitness_values = vector<double>(population_size, DBL_MAX);
    cities = readCitiesFromFile(seedfile);
    distances = initDistances(cities);
    population = initPopulation(cities);
    if (strcmp(crossover_method, "pmx") == 0)
        crossover = &GA::partiallyMappedCrossover;
    else if (strcmp(crossover_method, "cx") == 0)
        crossover = &GA::cycleCrossover;
    else // ox
        crossover = &GA::orderCrossover;
    total_threads = populations;
}

/**
 * Evaluate every chromosome in population
 * Parameter is_new_member is a boolean list to track if a chromosome
 * in population is created by crossover or directly from parents
 */
vector<double> GAP::evalPopulation(vector<bool> is_new_member, vector<vector<int>> pop, vector<double> f_values) {
    double temp;
    for (int i = 0; i < pop.size(); i++) {
        temp = fitness(pop[i]);
        f_values[i] = temp;
        if (f_values[i] < bestScore) {
            best_score_lock.lock();
            bestScore = f_values[i];
            best = pop[i];
            best_score_lock.unlock();
        }
        // TODO: lock result
        // if (is_new_member[i]) {
        //     result.push_back(bestScore);
        // }
    }
    return f_values;
}

mutex aa;
vector<vector<int>> GAP::exchange(vector<vector<int>> population) {
    int exchange_position = rand() % population.size();
    vector<int> s = population[exchange_position];
    // Lock the mutex and put solution to pool
    exchange_lock.lock();
    exchange_pool.push_back(s);
    ready_to_exchange++;
    exchange_lock.unlock();


    // Wait for other threads to push their solution to exchange_pool
    // int spin is used to prevent compiler optimization under -O3
    volatile int spin;
    while (ready_to_exchange < total_threads)
        spin++;


    // Pick a solution from pool
    exchange_lock.lock();
    int r = rand() % exchange_pool.size();
    s = exchange_pool[r];
    exchange_pool.erase(exchange_pool.begin() + r);
    // cout << ready_to_exchange << endl;
    ready_to_exchange--;
    // cout << ready_to_exchange << endl;
    // cout << exchange_pool.size() << endl;
    exchange_lock.unlock();

    
    // aa.lock();
    // cout << "exchange: ";
    // for (int i = 0; i < population[0].size(); i++)
    //     cout << population[exchange_position][i] << " ";
    // cout << endl << "to:       ";
    // for (int i = 0; i < population[0].size(); i++)
    //     cout << s[i] << " ";
    // cout << endl;
    // aa.unlock();

    // Replace solution
    population[exchange_position] = s;
    return population;
}

void GAP::threadHandler(int generations) {
    cout << "thread started" << endl;
    vector<vector<int>> thread_population = initPopulation(cities);
    vector<double> thread_fitness_values = vector<double>(population_size, DBL_MAX);
    for (int gen = 1; gen <= generations; gen++) {
        vector<int> a, b;
        vector<vector<int>> crossover_result;
        vector<vector<int>> new_population;
        // Save a boolean list to track if a chromosome is new or not, just for logging
        // vector<bool> new_member_list(population_size, false);

        for (int i = 0; i < population_size / 2; i++) {
            // Selection
            // a = rouletteWheel(thread_population);
            // b = rouletteWheel(thread_population);
            a = tournament(thread_population, thread_fitness_values);
            b = tournament(thread_population, thread_fitness_values);
            // Crossover
            if ((double) rand() / RAND_MAX < crossover_rate) {
                crossover_result = (this->*crossover)(a, b);
                new_population.insert(new_population.end(), crossover_result.begin(), crossover_result.end());
                // new_member_list[i * 2] = new_member_list[i * 2 + 1] = true;
            } else {
                new_population.push_back(a);
                new_population.push_back(b);
            }
        }
        // Mutation
        for (int i = 0; i < new_population.size(); i++) {
            if ((double) rand() / RAND_MAX < mutation_rate) {
                new_population[i] = mutation(new_population[i]);
                // new_member_list[i] = true;
            }
        }

        // Exchange solution with other populations
        if (gen % EXCHANGE_SOLUTION_GEN == 0) {
            new_population = exchange(new_population);
            cout << exchange_pool.size() << " at gen " << gen << endl; // TODO: Why 3 ?
        }

        // Apply population and fitness values
        thread_population = new_population;
        // evalPopulation(new_member_list);
        evalPopulation(vector<bool>(population_size, true), thread_population, thread_fitness_values);
    }
    // finished_lock.lock();
    // finished++;
    // finished_lock.unlock();
    cout << "thread end" << endl;
}

vector<double> GAP::run(int generations) {
    // Reference: https://stackoverflow.com/questions/17472827/create-thread-inside-class-with-function-from-same-class
    vector<thread> thread_list;
    for (int i = 0; i < total_threads; i++)
        thread_list.push_back(thread(&GAP::threadHandler, this, generations));

    // while (finished < total_threads)
    //     cout << "ready: " << ready_to_exchange << endl;

    for (int i = 0; i < thread_list.size(); i++)
        thread_list[i].join();
    // Record and log
    // if (gen % 100 == 0 || gen < 20) {
    //     cout << "Generation: " << gen << " Best score: " << bestScore << endl;
    // }
    cout << "All threads finished." << endl;
    // Fix the result size because crossover may cause different evaluations for each population
    int target_evaluations = generations * population_size * crossover_rate;
    int original_size = result.size();
    if (result.size() > target_evaluations)
        for (int i = 0; i < original_size - target_evaluations; i++)
            result.pop_back();
    else if (result.size() < target_evaluations)
        for (int i = 0; i < target_evaluations - original_size; i++)
            result.push_back(bestScore);
    cout << "==========================" << endl;
    cout << "Found Best: " << endl;
    for (int i = 0; i < best.size(); i++) {
        cout << best[i] << " ";
    }
    cout << endl;
    cout << "Distance: " << bestScore << endl;
    if (bestScore < 450) {
        cout << " XD";
    }
    cout << "Done." << endl;
    return result;
}