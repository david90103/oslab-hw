#include "ga.hpp"

/**
 * Function implementation of abstract class GA
 */

GA::GA(time_t randseed, int clusters, int encode_type, int population_s, double crossover_r, double mutation_r, char const *seedfile) {
    string line;
    // double max, min;
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
    /**
     * TODO: Normalize each dimension
     */
    // iris_normalized = iris;
    // for (int j = 0; j < dimension; j++) {
    //     max = -1;
    //     min = DBL_MAX;
    //     for (int i = 0; i < iris_normalized.size(); i++) {
    //         if (iris_normalized[i][j] < min) {
    //             min = iris_normalized[i][j];
    //         }
    //         if (iris_normalized[i][j] > max) {
    //             max = iris_normalized[i][j];
    //         }
    //     }
    //     for (int i = 0; i < iris_normalized.size(); i++) {
    //         iris_normalized[i][j] = (iris_normalized[i][j] - min) / (max - min);
    //     }
    // }
}

double GA::getBestScore() {
    return bestScore;
}

double GA::fitness_cluster_id(vector<int> arr) {
    double result = 0;
    vector<double> temp(dimension, 0);
    vector<vector<double>> cluster_avg(clusters, temp);
    vector<int> cluster_count(clusters, 0);
    for (int i = 0; i < arr.size(); i++) {
        cluster_count[arr[i]]++;
        for (int j = 0; j < dimension; j++) {
            cluster_avg[arr[i]][j] += iris[i][j];
        }
    }
    for (int i = 0; i < clusters; i++) {
        for (int j = 0; j < dimension; j++) {
            cluster_avg[i][j] /= cluster_count[i];
        }
    }
    for (int i = 0; i < arr.size(); i++) {
        for (int j = 0; j < dimension; j++) {
            result += pow(iris[i][j] - cluster_avg[arr[i]][j], 2);
        }
    }
    return result;
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
    printBestChromosome();
    cout << "Done." << endl;
    return result;
}

/**
 * Function implementation of ClusterIdGA
 */

void ClusterIdGA::initPopulation() {
    for (int i = 0; i < population_size; i++) {
        Chromosome temp;
        for (int j = 0; j < iris.size(); j++) {
            temp.cluster_id_encoded.push_back(rand() % this->clusters);
        }
        population.push_back(temp);
    }
}

double ClusterIdGA::fitness(Chromosome arr) {
    return fitness_cluster_id(arr.cluster_id_encoded);
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

void ClusterIdGA::printBestChromosome() {
    for (int i = 0; i < best.cluster_id_encoded.size(); i++) {
        cout << best.cluster_id_encoded[i] << " ";
    }
    cout << endl;
}

/**
 * Function implementation of CentroidsGA
 */

void CentroidsGA::initPopulation() {
    for (int i = 0; i < population_size; i++) {
        Chromosome temp;
        for (int j = 0; j < clusters; j++) {
            vector<double> t;
            for (int k = 0; k < dimension; k++) {
                /**
                 * TODO: Normalize each dimension 
                 */
                t.push_back((double) rand() / RAND_MAX * 8);
            }
            temp.centroids_encoded.push_back(t);
        }
        population.push_back(temp);
    }
}

inline double CentroidsGA::distance(vector<double> point, vector<double> centroid) {
    double sum = 0;
    for (int i = 0; i < dimension; i++) {
        sum += pow(point[i] - centroid[i], 2);
    }
    return sqrt(sum);
}

Chromosome CentroidsGA::convertToClusterId(Chromosome arr) {
    int nearest_cluster;
    double min_distance, temp;
    for (int i = 0; i < iris.size(); i++) {
        min_distance = DBL_MAX;
        for (int j = 0; j < clusters; j++) {
            temp = distance(iris[i], arr.centroids_encoded[j]);
            if (temp < min_distance) {
                min_distance = temp;
                nearest_cluster = j;
            }
        }
        arr.cluster_id_encoded.push_back(nearest_cluster);
    }
    return arr;
}

/**
 * Fitness computation process includes 2 phases
 * 1. Generate cluster id encoded chromosome by clustering each points to nearest centroids
 * 2. Call the fitness function with cluster_id_encoded chromosome as parameter
 */
double CentroidsGA::fitness(Chromosome arr) {
    return fitness_cluster_id(convertToClusterId(arr).cluster_id_encoded);
}

vector<Chromosome> CentroidsGA::crossover(Chromosome father, Chromosome mother) {
    int pivot = rand() % father.centroids_encoded.size();
    Chromosome child_a;
    Chromosome child_b;
    child_a.centroids_encoded = vector<vector<double>>(father.centroids_encoded.begin(), father.centroids_encoded.begin() + pivot);
    child_b.centroids_encoded = vector<vector<double>>(mother.centroids_encoded.begin(), mother.centroids_encoded.begin() + pivot);
    child_a.centroids_encoded.insert(child_a.centroids_encoded.end(), mother.centroids_encoded.begin() + pivot, mother.centroids_encoded.end());
    child_b.centroids_encoded.insert(child_b.centroids_encoded.end(), father.centroids_encoded.begin() + pivot, father.centroids_encoded.end());
    return vector<Chromosome> {child_a, child_b};
}

/**
 * Mutation of centroids encoded chromosome
 * for each dimension as v
 * v = v +/- 2 * delta * v
 * https://bit.ly/2FshxiP
 */
inline Chromosome CentroidsGA::mutation(Chromosome target) {
    int i = rand() % target.centroids_encoded.size();
    double delta;
    for (int j = 0; j < dimension; j++) {
        delta = (double) rand() / RAND_MAX;
        if (rand() & 1) {
            target.centroids_encoded[i][j] = target.centroids_encoded[i][j] + 2 * delta * target.centroids_encoded[i][j];
        } else {
            target.centroids_encoded[i][j] = target.centroids_encoded[i][j] - 2 * delta * target.centroids_encoded[i][j];
        }
    }
    return target;
}

void CentroidsGA::printBestChromosome() {
    best = convertToClusterId(best);
    for (int i = 0; i < best.cluster_id_encoded.size(); i++) {
        cout << best.cluster_id_encoded[i] << " ";
    }
    cout << bestScore << endl;
    for (int i = 0; i < best.centroids_encoded.size(); i++) {
        for (int j = 0; j < best.centroids_encoded[i].size(); j++) {
            cout << best.centroids_encoded[i][j] << " ";
        }
        cout << endl;
    }
}
