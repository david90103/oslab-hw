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
    evalPopulation();
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
 * TODO: Only evaluate new chromosomes from crossover
 */
void GA::evalPopulation() {
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
        result.push_back(bestScore);
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

        for (int i = 0; i < population_size / 2; i++) {
            // Selection
            a = rouletteWheel();
            b = rouletteWheel();
            // Crossover
            if ((double) rand() / RAND_MAX < crossover_rate) {
                crossover_result = crossover(a, b);
                new_population.insert(new_population.end(), crossover_result.begin(), crossover_result.end());
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
        evalPopulation();
        
        // Record and log
        // for (int i = 0; i < population_size; i++) {
        //     result.push_back(bestScore);
        // }
        if (gen % 10 == 0) {
            cout << "Generation: " << gen << " Best score: " << bestScore << endl;
        }
    }
    cout << "Done." << endl;
    return result;
}