#include "gap.hpp"

GAP::GAP(int bits, int population_s, double crossover_r, double mutation_r, char const *crossover_method, char const *seedfile) {
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
        crossover = &GAP::partiallyMappedCrossover;
    else if (strcmp(crossover_method, "cx") == 0)
        crossover = &GAP::cycleCrossover;
    else // ox
        crossover = &GAP::orderCrossover;
    evalPopulation(vector<bool>(population_size, true));
}

vector<double> GAP::run(int generations) {
    for (int gen = 1; gen <= generations; gen++) {
        vector<int> a, b;
        vector<vector<int>> crossover_result;
        vector<vector<int>> new_population;
        // Save a boolean list to track if a chromosome is new or not, just for logging
        vector<bool> new_member_list(population_size, false);

        for (int i = 0; i < population_size / 2; i++) {
            // Selection
            // a = rouletteWheel();
            // b = rouletteWheel();
            a = tournament();
            b = tournament();
            // Crossover
            if ((double) rand() / RAND_MAX < crossover_rate) {
                crossover_result = (this->*crossover)(a, b);
                new_population.insert(new_population.end(), crossover_result.begin(), crossover_result.end());
                new_member_list[i * 2] = new_member_list[i * 2 + 1] = true;
            } else {
                new_population.push_back(a);
                new_population.push_back(b);
            }
        }
        // Mutation
        for (int i = 0; i < new_population.size(); i++) {
            if ((double) rand() / RAND_MAX < mutation_rate) {
                new_population[i] = mutation(new_population[i]);
                new_member_list[i] = true;
            }
        }
        // Apply population and fitness values
        population = new_population;
        evalPopulation(new_member_list);
        
        // Record and log
        if (gen % 100 == 0 || gen < 20) {
            cout << "Generation: " << gen << " Best score: " << bestScore << endl;
        }
    }
    // Fix the result size because crossover may cause different evaluations for each population
    int target_evaluations = generations * population_size * crossover_rate;
    int original_size = result.size();
    if (result.size() > target_evaluations) {
        for (int i = 0; i < original_size - target_evaluations; i++) {
            result.pop_back();
        }
    }
    else if (result.size() < target_evaluations) {
        for (int i = 0; i < target_evaluations - original_size; i++) {
            result.push_back(bestScore);
        }
    }
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