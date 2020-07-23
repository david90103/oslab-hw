#include "aco.h"

ACO::ACO(int ants, double a, double b, double r, double q, char const *seedfile) {
    string line;
    srand(time(NULL));
    bestScore = 0.0;
    this->ants = ants;
    this->alpha = a;
    this->beta = b;
    this->rho = r;
    this->q = q;
    fitness_values_sum = 0.0;
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

double ACO::distance(vector<double> city_a, vector<double> city_b) {
    return sqrt(pow(city_a[0] - city_b[0], 2) + pow(city_a[1] - city_b[1], 2));
}

int ACO::rouletteWheel(int current_city, vector<int> candidate_city) {
    return candidate_city[rand() % candidate_city.size()];
}

void ACO::generatePath() {
    int current;
    vector<int> path;
    vector<int> candidate_city;
    population.clear();
    for (int i = 0; i < ants; i++) {
        current = 0;
        path.clear();
        candidate_city.clear();
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
        population.push_back(path);
    }
    /**
     * TODO: Evaluate Population
     */
}

void ACO::updatePheromone() {

}

vector<int> ACO::run(int iterations) {
    for (int iter = 1; iter <= iterations; iter++) {
        generatePath();
        updatePheromone();

        /**
         * TODO: Local Search
         * 2-opt search
         */

        // Record and log
        if (iter % 10 == 0) {
            cout << "Generation: " << iter << " Best score: " << bestScore << endl;
        }
    }
    
    cout << "Done." << endl;
    return vector<int>();
}