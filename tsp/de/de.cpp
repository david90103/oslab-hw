#include "de.hpp"

DE::DE(unsigned int randseed, int population_size, double crossover_rate, double f, char const *seedfile) {
    srand(randseed);
    this->bestScore = DBL_MAX;
    this->population_size = population_size;
    this->crossover_rate = crossover_rate;
    this->f = f;
    this->cities = readCitiesFromFile(seedfile);
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
            t.push_back(-0 + (double) rand() / RAND_MAX * (2 * 40));
        }
        population.push_back(t);
        fitness_values.push_back(evaluate(convertToPath(t)));
    }
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
    cout << "Distance: " << bestScore;
    if (bestScore < 480) {
        cout << " XD";
    }
    cout << endl;
    return result;
}
