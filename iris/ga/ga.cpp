#include "ga.hpp"

GA::GA(time_t randseed, int clusters, int encode_type, int population_s, double crossover_r, double mutation_r, char const *seedfile) {
    string line;
    srand(randseed);
    bestScore = INT_MAX;
    this->clusters = clusters;
    this->encode_type = encode_type;
    population_size = population_s;
    crossover_rate = crossover_r;
    mutation_rate = mutation_r;
    fitness_values_sum = INT_MAX;
    fitness_values = vector<double>(population_size, INT_MAX);
    ifstream fp(seedfile);
    while (getline(fp, line)) {
        if (isdigit(line.front())) {
            vector<double> split;
            string pattern = ",";
            line += pattern;
            for (int i = 0; i < line.size(); i++) {
                int pos = (int) line.find(pattern, i);
                if (pos != string::npos && isdigit(line[i])) {
                    split.push_back(atof(line.substr(i, pos - i).c_str()));
                    i = pos + (int) pattern.size() - 1;
                }
            }
            iris.push_back(split);
        }
    }
    dimension = iris[0].size();
}

void GA::printArray() {
    for (int i = 0; i < fitness_values.size(); i++) {
        cout << fitness_values[i] << " ";
    }
    cout << " Best: " << bestScore << endl;
}

double GA::getBestScore() {
    return bestScore;
}

/**
 * Evaluate every chromosome in population
 * Parameter is_new_member is a boolean list to track if a chromosome
 * in population is created by crossover or directly from parents
 */
void GA::evalPopulation(vector<bool> is_new_member) {
    double temp;
    fitness_values_sum = 0;
    for (int i = 0; i < population.size(); i++) {
        temp = fitness(population[i]);
        fitness_values[i] = temp;
        fitness_values_sum += temp;
        if (fitness_values[i] < bestScore) {
            bestScore = fitness_values[i];
            best = population[i];
        }
        // if (is_new_member[i]) {
        //     result.push_back(bestScore);
        // }
    }
}

/**
 * Selection using Roulette Wheel
 */
Chromosome GA::rouletteWheel() {
    int sum = 0;
    int target = (double) rand() / RAND_MAX * fitness_values_sum;
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
Chromosome GA::tournament() {
    int i = rand() % population_size;
    int j = rand() % population_size;
    if (fitness_values[i] >= fitness_values[j]) {
        return population[i];
    }
    return population[j];
}

vector<double> GA::run(int generations) {
    // Initialization
    initPopulation();
    evalPopulation(vector<bool>(population_size, true));

    for (int gen = 1; gen <= generations; gen++) {
        Chromosome a, b;
        vector<Chromosome> crossover_result;
        vector<Chromosome> new_population;
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
        result.push_back(bestScore);
        if (gen % 10 == 0) {
            cout << "Generation: " << gen << " Best score: " << bestScore << endl;
        }
    }
    cout << "Done." << endl;
    return result;
}

void ClusterIdGA::initPopulation() {
    for (int i = 0; i < population_size; i++) {
        Chromosome temp;
        for (int j = 0; j < iris.size(); j++) {
            temp.cluster_id_encoded.push_back(rand() % this->clusters);
        }
        population.push_back(temp);
    }
}

// void CentroidsGA::initPopulation() {
//     for (int i = 0; i < clusters; i++) {
//         Chromosome temp;
//         for (int j = 0; j < iris.size(); j++) {
//             vector<double> t;
//             for (int k = 0; k < dimension; k++) {
//                 t.push_back((double) rand() / RAND_MAX);
//             }
//             temp.centroids_encoded.push_back(t);
//         }
//         population.push_back(temp);
//     }
// }

/**
 * Objective function
 */
double ClusterIdGA::fitness(Chromosome arr) {
    double result = 0;
    vector<double> temp(dimension, 0);
    vector<vector<double>> cluster_avg(clusters, temp);
    vector<int> cluster_count(clusters, 0);
    for (int i = 0; i < arr.cluster_id_encoded.size(); i++) {
        cluster_count[arr.cluster_id_encoded[i]]++;
        for (int j = 0; j < dimension; j++) {
            cluster_avg[arr.cluster_id_encoded[i]][j] += iris[i][j];
        }
    }
    for (int i = 0; i < clusters; i++) {
        for (int j = 0; j < dimension; j++) {
            cluster_avg[i][j] /= cluster_count[i];
        }
    }
    for (int i = 0; i < arr.cluster_id_encoded.size(); i++) {
        for (int j = 0; j < dimension; j++) {
            result += pow(iris[i][j] - cluster_avg[arr.cluster_id_encoded[i]][j], 2);
        }
    }
    return result;
}

vector<Chromosome> ClusterIdGA::crossover(Chromosome father, Chromosome mother) {
    int pivot = rand() % father.cluster_id_encoded.size();
    Chromosome child_a;
    Chromosome child_b;
    child_a.cluster_id_encoded = vector<int>(father.cluster_id_encoded.begin(), father.cluster_id_encoded.begin() + pivot);
    child_b.cluster_id_encoded = vector<int>(mother.cluster_id_encoded.begin(), mother.cluster_id_encoded.begin() + pivot);
    child_a.cluster_id_encoded.insert(child_a.cluster_id_encoded.end(), mother.cluster_id_encoded.begin() + pivot, mother.cluster_id_encoded.end());
    child_b.cluster_id_encoded.insert(child_b.cluster_id_encoded.end(), father.cluster_id_encoded.begin() + pivot, father.cluster_id_encoded.end());
    return vector<Chromosome> {child_a, child_b};
}


inline Chromosome ClusterIdGA::mutation(Chromosome target) {
    int i = rand() % target.cluster_id_encoded.size();
    int new_cluster = rand() % clusters;
    while (new_cluster == target.cluster_id_encoded[i]) {
        new_cluster = rand() % clusters;
    }
    target.cluster_id_encoded[i] = new_cluster;
    return target;
}
