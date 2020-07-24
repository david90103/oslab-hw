#include "aco.h"

ACO::ACO(unsigned int randseed, int ants, double a, double b, double r, double q, char const *seedfile) {
    string line;
    srand(randseed);
    bestScore = 99999999.9;
    this->ants = ants;
    this->alpha = a;
    this->beta = b;
    this->rho = r;
    this->q = q;
    fitness_values = vector<double>(ants, 0.0);
    // Read cities from seedfile
    ifstream fp(seedfile);
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
    // Initialize distance table
    for (int i = 0; i < cities.size(); i++) {
        distances.push_back(vector<double>());
        for (int j = 0; j < cities.size(); j++) {
            distances[i].push_back(distance(cities[i], cities[j]));
        }
    }
    // Initialize pheromone table
    vector<double> temp(cities.size(), 0.001);
    for (int j = 0; j < cities.size(); j++) {
        pheromone.push_back(temp);
    }
}

double ACO::getBestScore() {
    return bestScore;
}

double ACO::distance(vector<double> city_a, vector<double> city_b) {
    return sqrt(pow(city_a[0] - city_b[0], 2) + pow(city_a[1] - city_b[1], 2));
}

double ACO::evaluate(vector<int> path) {
    double sum = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        sum += distances[path[i]][path[i + 1]];
    }
    return sum;
}

int ACO::rouletteWheel(int current_city, vector<int> candidate_city) {
    double temp, target;
    double sum = 0;
    vector<double> table;
    for (int i = 0; i < candidate_city.size(); i++) {
        temp = pow(pheromone[current_city][candidate_city[i]], alpha) * pow(1 / distances[current_city][candidate_city[i]], beta);
        sum += temp;
        table.push_back(temp);
    }
    for (int i = 0; i < table.size(); i++) {
        table[i] = table[i] / sum;
    }
    target = (double) rand() / RAND_MAX;
    sum = 0;
    for (int i = 0; i < table.size(); i++) {
        sum += table[i];
        if (sum > target) {
            return candidate_city[i];
        }
    }
    return candidate_city[candidate_city.size() - 1];
}

void ACO::generatePath() {
    int current, first;
    double score;
    vector<int> path;
    vector<int> candidate_city;
    population.clear();
    for (int i = 0; i < ants; i++) {
        path.clear();
        candidate_city.clear();
        current = rand() % cities.size();
        first = current;
        for (int j = 0; j < cities.size(); j++) {
            candidate_city.push_back(j);
        }
        for (int j = 0; j < cities.size(); j++) {
            path.push_back(current);
            // Remove current city
            for (vector<int>::iterator iter = candidate_city.begin(); iter != candidate_city.end(); iter++) {
                if(*iter == current) {
                    candidate_city.erase(iter);
                    break;
                }
            }
            if (candidate_city.size() > 0) {
                current = rouletteWheel(current, candidate_city);
            }
        }
        path.push_back(first);
        population.push_back(path);
    }    
    // Evaluate Population
    for (int i = 0; i < ants; i++) {
        score = evaluate(population[i]);
        fitness_values[i] = score;
        if (score < bestScore) {
            bestScore = score;
            best = path;
        }
        result.push_back(bestScore);
    }
}

void ACO::updatePheromone() {
    vector<vector<double>> add_pheromone;
    vector<double> temp(cities.size(), 0);
    for (int i = 0; i < cities.size(); i++) {
        add_pheromone.push_back(temp);
    }
    for (int i = 0; i < ants; i++) {
        for (int j = 0; j < population[i].size() - 1; j++) {
            add_pheromone[population[i][j]][population[i][j + 1]] += q / fitness_values[i];
        }
    }
    for (int i = 0; i < add_pheromone.size(); i++) {
        for (int j = 0; j < add_pheromone[i].size() - 1; j++) {
            pheromone[i][j] *= 1.00 - rho;
            pheromone[i][j] += add_pheromone[i][j];
        }
    }
    // Sync a->b with b->a
    for (int i = 0; i < pheromone.size(); i++) {
        for (int j = 0; j < pheromone[i].size(); j++) {
            pheromone[i][j] = pheromone[j][i] = (pheromone[i][j] + pheromone[j][i]) / 2;
        }
    }
}

vector<double> ACO::run(int iterations) {
    for (int iter = 1; iter <= iterations; iter++) {
        generatePath();
        updatePheromone();

        /**
         * TODO: Local Search
         * 2-opt search
         */

        // Record and log
        // if (iter % 10 == 0) {
        //     cout << "Generation: " << iter << " Best score: " << bestScore << endl;
        // }
    }
    for (int i = 0; i < best.size(); i++) {
        cout << best[i] << " ";
    }
    cout << " Disatance: " << bestScore << endl;
    cout << "Done." << endl;
    return result;
}