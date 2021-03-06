#include "de1dc.hpp"

DE1DC::DE1DC(unsigned int randseed, int population_size, double crossover_rate, double f, char const *seedfile) {
    srand(randseed);
    this->bestScore = DBL_MAX;
    this->population_size = population_size;
    this->crossover_rate = crossover_rate;
    this->f = f;
    this->cities = readCitiesFromFile(seedfile);
    this->cities_normalized = normalize(this->cities);
    this->dimension = cities.size();
    this->distances = initDistances(this->cities);
    this->distances_normalized = initDistances(this->cities_normalized);
    // Initialize population
    vector<double> t;
    for (int i = 0; i < population_size; i++) {
        t.clear();
        for (int j = 0; j < dimension; j++)
            t.push_back((double) rand() / RAND_MAX);
            // t.push_back((j + (double) rand() / RAND_MAX * 0.1) / dimension);
        sort(t.begin(), t.end());
        population.push_back(t);
        fitness_values.push_back(evaluateNormalized(convertToPathNormalized(t)));
    }
    // Initialize the circle position
    initCircle(cities_normalized);
}

vector<vector<double>> DE1DC::mutation(vector<vector<double>> population) {
    vector<vector<double>> v_arr;
    for (int i = 0; i < population.size(); i++) {
        vector<double> temp;
        int r1 = rand() % population_size;
        int r2 = rand() % population_size;
        int r3 = rand() % population_size;
        for (int j = 0; j < dimension; j++) {
            // Boundary check
            double t = population[r1][j] + f * (population[r2][j] - population[r3][j]);
            if (t > 1)
                t -= 1;
            if (t < 0)
                t += 1;
            temp.push_back(t);
        }
        v_arr.push_back(temp);
    }
    return v_arr;
}

vector<vector<double>> DE1DC::crossover(vector<vector<double>> population, vector<vector<double>> v_arr) {
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

vector<double> DE1DC::run(int iterations) {
    for (int iter = 1; iter <= iterations; iter++) {
        vector<vector<double>> v_arr = mutation(population);
        vector<vector<double>> u_arr = crossover(population, v_arr);
        // Selection
        for (int i = 0; i < population_size; i++) {
            vector<int> path = convertToPathNormalized(u_arr[i]);
            double eval = evaluateNormalized(path);
            if (eval <= fitness_values[i]) {
                fitness_values[i] = eval;
                population[i] = u_arr[i];
            }
            if (eval < bestScore) {
                bestScore = eval;
                global_best = u_arr[i];
                best = path;
            }
            // Uncomment this to save result in every evaluation
            // result.push_back(evaluate(best));
        }
        // Record and log
        result.push_back(evaluate(best));
        if (iter % 100 == 0 || iter < 20) {
            cout << "Iteration: " << iter << " Best fitness: " << bestScore << " Best score: " << evaluate(best) << endl;
        }
    }
    cout << "==========================" << endl;
    cout << "Found Best: " << endl;
    for (int i = 0; i < best.size(); i++) {
        cout << best[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < global_best.size(); i++) {
        // cout << global_best[i] << " ";
        cout << centerX + radius_normalized * cos(global_best[i] * 2 * PI) 
             << " " << centerY + radius_normalized * sin(global_best[i] * 2 * PI) 
             << " ";
        cout << i << endl;
    }
    cout << endl;
    cout << "Distance: " << evaluate(best);
    if (evaluate(best) < 440) {
        cout << " XD";
    }
    cout << endl;
    return result;
}
