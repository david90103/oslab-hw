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
    vector<double> temp(cities.size(), PHEROMONE_INIT);
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

int ACO::rouletteWheelwithRBP(int current_city, vector<int> candidate_city) {
    double temp, target;
    double sum = 0;
    vector<double> table;
    vector<int> bestThree;
    vector<double> bestThreeProb;
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
    /**
     * Random on Best Probability ACO
     * If more than three city, pick three best and random choose to diversify the search
     * Otherwise, use Roulette Wheel
     * http://www.ijmlc.org/vol8/670-L0121F.pdf
     */
    /**
     * USELESS: NO! DO NOT USE THIS
     */
    // if (candidate_city.size() > 3) {
    //     bestThree.push_back(candidate_city[0]);
    //     bestThree.push_back(candidate_city[1]);
    //     bestThree.push_back(candidate_city[2]);
    //     bestThreeProb.push_back(table[0]);
    //     bestThreeProb.push_back(table[1]);
    //     bestThreeProb.push_back(table[2]);
    //     for (int i = 3; i < candidate_city.size(); i++) {
    //         if (bestThreeProb[0] < bestThreeProb[1] && bestThreeProb[0] < bestThreeProb[2]) {
    //             temp = 0;
    //         }
    //         else if (bestThreeProb[1] < bestThreeProb[0] && bestThreeProb[1] < bestThreeProb[2]) {
    //             temp = 1;
    //         }
    //         else {
    //             temp = 2;
    //         }
    //         bestThree[temp] = candidate_city[i];
    //         bestThreeProb[temp] = table[i];
    //     }
    //     return bestThree[rand() % 3];
    // } else {
        for (int i = 0; i < table.size(); i++) {
            sum += table[i];
            if (sum > target) {
                return candidate_city[i];
            }
        }
    // }
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
                current = rouletteWheelwithRBP(current, candidate_city);
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
            best = population[i];
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
    /**
     * Pheromone Update
     * TODO: Try to update pheromone during generating path
     */
    for (int i = 0; i < ants; i++) {
        for (int j = 0; j < population[i].size() - 1; j++) {
            // Sync a->b with b->a
            add_pheromone[population[i][j]][population[i][j + 1]] += q / fitness_values[i];
            add_pheromone[population[i][j + 1]][population[i][j]] += q / fitness_values[i];
        }
    }
    for (int i = 0; i < pheromone.size(); i++) {
        for (int j = 0; j < pheromone[i].size(); j++) {
            pheromone[i][j] = ((1 - rho) * pheromone[i][j]) + add_pheromone[i][j];
        }
    }
    /**
     * Local Pheromone Update
     * The main goal of the local update is to diversify the
     * search performed by subsequent ants during an iteration
     * https://courses.cs.ut.ee/all/MTAT.03.238/2011K/uploads/Main/04129846.pdf
     */
    for (int i = 0; i < pheromone.size(); i++) {
        for (int j = 0; j < pheromone[i].size(); j++) {
            pheromone[i][j] = ((1 - rho) * pheromone[i][j]) + (rho * PHEROMONE_INIT);
        }
    }
    /**
     * Global Pheromone Update
     * This update will keep the global best path information
     * http://people.idsia.ch/~luca/acs-ec97.pdf
     */
    for (int i = 0; i < add_pheromone.size(); i++) {
        for (int j = 0; j < add_pheromone[i].size(); j++) {
            add_pheromone[i][j] = 0;
        }
    }
    for (int i = 0; i < best.size() - 1; i++) {
        // Sync a->b with b->a
        add_pheromone[best[i]][best[i + 1]] += 1 / bestScore;
        add_pheromone[best[i + 1]][best[i]] += 1 / bestScore;
    }
    for (int i = 0; i < pheromone.size(); i++) {
        for (int j = 0; j < pheromone[i].size(); j++) {
            pheromone[i][j] = (1 - GLOBAL_SEARCH_ALPHA) * pheromone[i][j] + GLOBAL_SEARCH_ALPHA * add_pheromone[i][j];
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
    cout << " Disatance: " << bestScore;
    if (bestScore < 433) {
        cout << " XD";
    }
    cout << endl;
    return result;
}