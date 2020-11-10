#include "de.hpp"

DE::DE(unsigned int randseed, int population_size, double crossover_rate, double f, char const *seedfile) {
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

int DE::findNearest(vector<double> position, vector<int> candidate_cities) {
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

vector<int> DE::convertToPath(vector<double> coordinates) {
    vector<int> path;
    vector<int> candidate_cities;
    for (int i = 0; i < cities.size(); i++) 
        candidate_cities.push_back(i);
    // Random select start index
    // int offset = rand() % cities.size();
    for (int i = 0; i < cities.size(); i++) {
        // vector<double> position = {coordinates[(offset + 2 * i) % cities.size()], coordinates[(offset + 2 * i + 1) % cities.size()]};
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

double DE::distance(vector<double> a, vector<double> b) {
    return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
}

double DE::evaluateWithDistance(vector<double> coordinates, vector<int> path) {
    double sum_of_distance = 0.0;
    for (int i = 0; i < cities.size(); i++)
        sum_of_distance += distance(cities[path[i]], {coordinates[i * 2], coordinates[i * 2 + 1]});
    return evaluate(path) + sum_of_distance / 100;
}

double DE::evaluate(vector<int> path) {
    double sum = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        sum += distances[path[i]][path[i + 1]];
    }
    return sum;
}

vector<vector<double>> DE::mutation(vector<vector<double>> population) {
    vector<vector<double>> v_arr;
    for (int i = 0; i < population.size(); i++) {
        vector<double> temp;
        int r1 = rand() % population_size;
        int r2 = rand() % population_size;
        int r3 = rand() % population_size;
        for (int j = 0; j < dimension; j++) {
            // Boundary check
            double t = population[r1][j] + f * (population[r2][j] - population[r3][j]);
            if (t > 100)
                t = 100;
            if (t < 0)
                t = 0;
            temp.push_back(t);
        }
        v_arr.push_back(temp);
    }
    return v_arr;
}

vector<vector<double>> DE::crossover(vector<vector<double>> population, vector<vector<double>> v_arr) {
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

vector<double> DE::run(int iterations) {
    for (int iter = 1; iter <= iterations; iter++) {
        vector<vector<double>> v_arr = mutation(population);
        vector<vector<double>> u_arr = crossover(population, v_arr);
        // Selection
        for (int i = 0; i < population_size; i++) {
            vector<int> path = convertToPath(u_arr[i]);
            // double eval = evaluateWithDistance(u_arr[i], path);
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
            cout << "Iteration: " << iter << " Best fitness: " << bestScore << " Best score: " << evaluate(best) << endl;
        }
    }
    cout << "==========================" << endl;
    cout << "Found Best: " << endl;
    // for (int i = 0; i < best.size(); i++) {
    //     cout << distance(cities[best[i]], {global_best[i*2], global_best[i*2+1]}) << " ";
    // }
    // cout << endl;
    for (int i = 0; i < best.size(); i++) {
        cout << best[i] << " ";
    }
    cout << endl;
    // for (int i = 0; i < global_best.size(); i++) {
    //     cout << global_best[i] << " ";
    //     if (i & 1)
    //         cout << i/2 << endl;
    // }
    // cout << endl;
    cout << "Disatance: " << bestScore;
    if (bestScore < 480) {
        cout << " XD";
    }
    cout << endl;
    return result;
}
