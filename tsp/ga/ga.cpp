#include "ga.hpp"

GA::GA(int bits, int population_s, double crossover_r, double mutation_r, char const *crossover_method, char const *seedfile) {
    srand(time(NULL));
    bestScore = DBL_MAX;
    population_size = population_s;
    crossover_rate = crossover_r;
    mutation_rate = mutation_r;
    fitness_values_sum = 0;
    fitness_values = vector<double>(population_size, DBL_MAX);
    cities = readCitiesFromFile(seedfile);
    distances = initDistances(cities);
    population = initPopulation(cities);
    if (strcmp(crossover_method, "pmx") == 0)
        crossover = partiallyMappedCrossover;
    else if (strcmp(crossover_method, "cx") == 0)
        crossover = cycleCrossover;
    else if (strcmp(crossover_method, "ox") == 0)
        crossover = orderCrossover;
    evalPopulation(vector<int>(population_size, true));
}

vector<vector<double>> GA::readCitiesFromFile(char const *seedfile) {
    vector<vector<double>> cities;
    ifstream fp(seedfile);
    string line;
    while (getline(fp, line)) {
        if (isdigit(line.front())) {
            vector<double> split;
            string pattern = " ";
            line += pattern;
            for (int i = 0; i < line.size(); i++) {
                int pos = (int) line.find(pattern, i);
                if (pos != string::npos) {
                    split.push_back(atof(line.substr(i, pos - i).c_str()));
                    i = pos + (int) pattern.size() - 1;
                }
            }
            split.erase(split.begin());
            cities.push_back(split);
        }
    }
    return cities;
}

vector<vector<double>> GA::initDistances(vector<vector<double>> cities) {
    vector<vector<double>> distances;
    for (int i = 0; i < cities.size(); i++) {
        distances.push_back(vector<double>());
        for (int j = 0; j < cities.size(); j++) {
            distances[i].push_back(distance(cities[i], cities[j]));
        }
    }
    return distances;
}

vector<vector<int>> GA::initPopulation(vector<vector<double>> cities) {
    vector<vector<int>> population;
    vector<int> candidate_cities;
    vector<int> temp_path;
    for (int i = 0; i < population_size; i++) {
        temp_path.clear();
        candidate_cities.clear();
        for (int j = 0; j < cities.size(); j++) 
            candidate_cities.push_back(j);
        for (int j = 0; j < cities.size(); j++) {
            int r = rand() % candidate_cities.size();
            temp_path.push_back(candidate_cities[r]);
            candidate_cities.erase(candidate_cities.begin() + r);
        }
        temp_path.push_back(temp_path[0]);
        population.push_back(temp_path);
    }
    return population;
}

double GA::distance(vector<double> a, vector<double> b) {
    return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
}

/**
 * Evaluate every chromosome in population
 * Parameter is_new_member is a intean list to track if a chromosome
 * in population is created by crossover or directly from parents
 */
void GA::evalPopulation(vector<int> is_new_member) {
    int temp;
    fitness_values_sum = 0;
    for (int i = 0; i < population.size(); i++) {
        temp = fitness(population[i]);
        fitness_values[i] = temp;
        fitness_values_sum += temp;
        if (fitness_values[i] < bestScore) {
            bestScore = fitness_values[i];
            best = population[i];
        }
        if (is_new_member[i]) {
            result.push_back(bestScore);
        }
    }
}

/**
 * Objective function
 */
int GA::fitness(vector<int> path) {
    double sum = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        sum += distances[path[i]][path[i + 1]];
    }
    return sum;
}

/**
 * Selection using Roulette Wheel
 */
vector<int> GA::rouletteWheel() {
    int sum = 0;
    int target = rand() % fitness_values_sum;
    for (int i = 0; i < population_size; i++) {
        sum += fitness_values[i];
        if (sum >= target) {
            return population[i];
        }
    }
    return population[population_size - 1];
}

/**
 * Selection using Tournament Selection
 */
vector<int> GA::tournament() {
    int i = rand() % population_size;
    int j = rand() % population_size;
    if (fitness_values[i] >= fitness_values[j]) {
        return population[i];
    }
    return population[j];
}

vector<vector<int>> GA::partiallyMappedCrossover(vector<int> father, vector<int> mother) {
    
}

vector<vector<int>> GA::cycleCrossover(vector<int> father, vector<int> mother) {
    
}

vector<vector<int>> GA::orderCrossover(vector<int> father, vector<int> mother) {
    
}

inline vector<int> GA::mutation(vector<int> target) {
    // We don't want to mutate the first and last position
    int i = rand() % (target.size() - 2) + 1;
    int temp = target[i];
    target[i] = target[target.size() - i];
    target[target.size() - i] = temp;
    return target;
}

vector<double> GA::run(int generations) {
    for (int gen = 1; gen <= generations; gen++) {
        vector<int> a, b;
        vector<vector<int>> crossover_result;
        vector<vector<int>> new_population;
        // Save a intean list to track if a chromosome is new or not, just for logging
        vector<int> new_member_list(population_size, false);

        for (int i = 0; i < population_size / 2; i++) {
            // Selection
            a = rouletteWheel();
            b = rouletteWheel();
            // Crossover
            if ((double) rand() / RAND_MAX < crossover_rate) {
                crossover_result = (*crossover)(a, b);
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
        if (gen % 10 == 0) {
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
    cout << "Done." << endl;
    return result;
}