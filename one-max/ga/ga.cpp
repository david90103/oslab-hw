#include "ga.h"

GA::GA(int bits, int population_s, double crossover_r, double mutation_r, char const *seedfile) {
    char c;
    vector<bool> tmp;
    FILE *fp = fopen(seedfile, "r");
    srand(time(NULL));
    bestScore = 0;
    size = bits;
    population_size = population_s;
    crossover_rate = crossover_r;
    mutation_rate = mutation_r;
    fitness_values_sum = 0;
    fitness_values = vector<int>(population_size, 0);
    for (int i = 0; i < population_size; i++) {
        tmp = vector<bool>();
        for (int j = 0; j < size; j++) {
            if (fp) {
                fscanf(fp, "%c", &c);
                tmp.push_back((c == '0') ? false : true);
            } else {
                tmp.push_back(rand() & 1);
            }
        }
        population.push_back(tmp);
    }
    if (fp) {
        fclose(fp);
    }
    evalPopulation(vector<bool>(population_size, true));
}

void GA::printArray() {
    for (int i = 0; i < fitness_values.size(); i++) {
        cout << fitness_values[i] << " ";
    }
    cout << " Best: " << bestScore << endl;
}

int GA::getBestScore() {
    return bestScore;
}

/**
 * Evaluate every chromosome in population
 * Parameter is_new_member is a boolean list to track if a chromosome
 * in population is created by crossover or directly from parents
 */
void GA::evalPopulation(vector<bool> is_new_member) {
    int temp;
    fitness_values_sum = 0;
    for (int i = 0; i < population.size(); i++) {
        temp = fitness(population[i]);
        fitness_values[i] = temp;
        fitness_values_sum += temp;
        if (fitness_values[i] > bestScore) {
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
int GA::fitness(vector<bool> arr) {
    int result = 0;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i]) {
            result++;
        }
    }
    return result;
}

/**
 * Selection using Roulette Wheel
 */
vector<bool> GA::rouletteWheel() {
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
vector<bool> GA::tournament() {
    int i = rand() % population_size;
    int j = rand() % population_size;
    if (fitness_values[i] >= fitness_values[j]) {
        return population[i];
    }
    return population[j];
}

/**
 * Crossover function, returns two children in a vector
 */
vector<vector<bool>> GA::crossover(vector<bool> father, vector<bool> mother) {
    int pivot = rand() % father.size();
    vector<bool> child_a(father.begin(), father.begin() + pivot);
    vector<bool> child_b(mother.begin(), mother.begin() + pivot);
    child_a.insert(child_a.end(), mother.begin() + pivot, mother.end());
    child_b.insert(child_b.end(), father.begin() + pivot, father.end());
    return vector<vector<bool>> {child_a, child_b};
}

inline vector<bool> GA::mutation(vector<bool> target) {
    int i = rand() % target.size();
    target[i] = !target[i];
    return target;
}

vector<int> GA::run(int generations) {
    for (int gen = 1; gen <= generations; gen++) {
        vector<bool> a, b;
        vector<vector<bool>> crossover_result;
        vector<vector<bool>> new_population;
        // Save a boolean list to track if a chromosome is new or not, just for logging
        vector<bool> new_member_list(population_size, false);

        for (int i = 0; i < population_size / 2; i++) {
            // Selection
            a = rouletteWheel();
            b = rouletteWheel();
            // Crossover
            if ((double) rand() / RAND_MAX < crossover_rate) {
                crossover_result = crossover(a, b);
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