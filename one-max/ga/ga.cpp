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

void GA::evalPopulation() {
    for (int i = 0; i < population.size(); i++) {
        fitness_values[i] = fitness(population[i]);
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

vector<bool> GA::rouletteWheel() {
    int sum = 0;
    int target;
    for (int i = 0; i < population_size; i++) {
        sum += fitness_values[i];
    }
    target = rand() % sum;
    sum = 0;
    for (int i = 0; i < population_size; i++) {
        sum += fitness_values[i];
        if (sum >= target) {
            return population[i];
        }
    }
    return population[population_size - 1];
}

vector<bool> GA::tournament() {
    int i = rand() % population_size;
    int j = rand() % population_size;
    if (fitness_values[i] >= fitness_values[j]) {
        return population[i];
    }
    return population[j];
}

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
            a = tournament();
            b = tournament();
            if (rand() / RAND_MAX < crossover_rate) {
                // Crossover
                crossover_result = crossover(a, b);
                new_population.insert(new_population.end(), crossover_result.begin(), crossover_result.end());
            } else {
                new_population.push_back(a);
                new_population.push_back(b);
            }
        }
        // Mutation
        for (int i = 0; i < new_population.size(); i++) {
            if (rand() / RAND_MAX < mutation_rate) {
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