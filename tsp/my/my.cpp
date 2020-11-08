#include "my.hpp"

My::My(unsigned int randseed, int population_size, double crossover_rate, double f, char const *seedfile) {
    string line;
    srand(randseed);
    bestScore = DBL_MAX;
    this->population_size = population_size;
    this->crossover_rate = crossover_rate;
    this->f = f;
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
    this->dimension = 2 * cities.size();
    // Initialize distance table
    for (int i = 0; i < cities.size(); i++) {
        distances.push_back(vector<double>());
        for (int j = 0; j < cities.size(); j++) {
            distances[i].push_back(distance(cities[i], cities[j]));
        }
    }
    // Initialize population
    vector<double> t;
    for (int i = 0; i < population_size; i++) {
        t.clear();
        for (int j = 0; j < dimension; j++) {
            t.push_back(-30 + (double) rand() / RAND_MAX * (2 * 30));
        }
        population.push_back(t);
        fitness_values.push_back(evaluate(convertToPath(t)));
    }
}

int My::findNearest(vector<double> position, vector<int> candidate_cities) {
    vector<double> distance_to_position;
    double min_distance = DBL_MAX;
    int min_city = -1;
    for (int i = 0; i < candidate_cities.size(); i++) {
        double dist = distance(position, cities[candidate_cities[i]]);
        if (dist < min_distance) {
            min_distance = dist;
            min_city = candidate_cities[i];
        }
    }
    return min_city;
}

vector<int> My::convertToPath(vector<double> coordinates) {
    vector<int> path;
    vector<int> candidate_cities;
    for (int i = 0; i < cities.size(); i++) 
        candidate_cities.push_back(i);
    for (int i = 0; i < cities.size(); i++) {
        vector<double> position = {coordinates[2 * i], coordinates[2 * i + 1]};
        int next_city = findNearest(position, candidate_cities);
        path.push_back(next_city);
        // Remove city from list
        for (vector<int>::iterator iter = candidate_cities.begin(); iter != candidate_cities.end(); iter++) {
            if (*iter == next_city) {
                candidate_cities.erase(iter);
                break;
            }
        }
    }
    path.push_back(path[0]);
    return path;
}

double My::distance(vector<double> a, vector<double> b) {
    return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
}

double My::evaluate(vector<int> path) {
    double sum = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        sum += distances[path[i]][path[i + 1]];
    }
    return sum;
}

vector<vector<double>> My::mutation(vector<vector<double>> population) {
    vector<vector<double>> v_arr;
    for (int i = 0; i < population.size(); i++) {
        vector<double> temp;
        int r1 = rand() % population_size;
        int r2 = rand() % population_size;
        int r3 = rand() % population_size;
        for (int j = 0; j < dimension; j++) {
            temp.push_back(population[r1][j] + f * (population[r2][j] - population[r3][j]));
        }
        v_arr.push_back(temp);
    }
    return v_arr;
}

vector<vector<double>> My::crossover(vector<vector<double>> population, vector<vector<double>> v_arr) {
    vector<vector<double>> u_arr;
    for (int i = 0; i < population.size(); i++) {
        vector<double> temp;
        for (int j = 0; j < dimension; j++) {
            if (i == j || (double) rand() / RAND_MAX <= crossover_rate) {
                temp.push_back(v_arr[i][j]);
            } else {
                temp.push_back(population[i][j]);
            }
        }
        u_arr.push_back(temp);
    }
    return u_arr;
}

vector<double> My::run(int iterations) {
    for (int iter = 1; iter <= iterations; iter++) {
        vector<vector<double>> v_arr = mutation(population);
        vector<vector<double>> u_arr = crossover(population, v_arr);
        // Selection
        for (int i = 0; i < population_size; i++) {
            vector<int> path = convertToPath(u_arr[i]);
            double eval = evaluate(path);
            if (eval <= fitness_values[i]) {
                fitness_values[i] = eval;
                population[i] = u_arr[i];
            }
            if (eval < bestScore) {
                bestScore = eval;
                global_best = u_arr[i];
                best = path;
            }
        }
        // Record and log
        result.push_back(bestScore);
        if (iter % 100 == 0) {
            cout << "Iteration: " << iter << " Best score: " << bestScore << endl;
        }
    }
    for (int i = 0; i < best.size(); i++) {
        cout << best[i] << " ";
    }
    cout << " Disatance: " << bestScore;
    if (bestScore < 480) {
        cout << " XD";
    }
    cout << endl;
    return result;
}
