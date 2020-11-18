#include "pso.hpp"

PSO::PSO(unsigned int randseed, int population_size, double w, double c1, double c2, char const *seedfile) {
    srand(randseed);
    this->bestScore = DBL_MAX;
    this->bestFitness = DBL_MAX;
    this->population_size = population_size;
    this->w = w;
    this->c1 = c1;
    this->c2 = c2;
    this->cities = readCitiesFromFile(seedfile);
    this->cities_normalized = normalize(this->cities);
    this->dimension = 2 * cities.size();
    this->objective_values = vector<double>(population_size, DBL_MAX);
    this->individual_bests = vector<double>(population_size, DBL_MAX);
    this->animation_file_count = 0;
    // Initialize distance table
    for (int i = 0; i < cities.size(); i++) {
        distances.push_back(vector<double>());
        for (int j = 0; j < cities.size(); j++) {
            distances[i].push_back(distance(cities[i], cities[j]));
        }
    }
    // Initialize normalized distance table
    for (int i = 0; i < cities_normalized.size(); i++) {
        distances_normalized.push_back(vector<double>());
        for (int j = 0; j < cities_normalized.size(); j++) {
            distances_normalized[i].push_back(distance(cities_normalized[i], cities_normalized[j]));
        }
    }
    // Initialize population
    vector<double> t;
    vector<double> temp(dimension, 0);
    for (int i = 0; i < population_size; i++) {
        t.clear();
        for (int j = 0; j < dimension; j++) {
            // t.push_back(-0 + (double) rand() / RAND_MAX * (2 * 40));
            t.push_back((double) rand() / RAND_MAX);
        }
        population.push_back(t);
        individual_bests_pos.push_back(temp);
        t.clear();
        for (int j = 0; j < dimension; j++) {
            t.push_back(-1 + (double) rand() / RAND_MAX * 2); // -1 ~ 1
        }
        velocities.push_back(t);
    }
}

vector<vector<double>> PSO::normalize(vector<vector<double>> cities) {
    double max_x = -1;
    double max_y = -1;
    double min_x = DBL_MAX;
    double min_y = DBL_MAX;
    for (int i = 0; i < cities.size(); i++) {
        if (cities[i][0] < min_x) 
            min_x = cities[i][0];
        if (cities[i][1] < min_y) 
            min_y = cities[i][1];
        if (cities[i][0] > max_x) 
            max_x = cities[i][0];
        if (cities[i][1] > max_y) 
            max_y = cities[i][1];
    }
    for (int i = 0; i < cities.size(); i++) {
        cities[i][0] = (cities[i][0] - min_x) / (max_x - min_x);
        cities[i][1] = (cities[i][1] - min_y) / (max_y - min_y);
    }
    return cities;
}

void PSO::updateVelocity() {
    for (int i = 0; i < population_size; i++) {
        for (int j = 0; j < dimension; j++) {
            velocities[i][j] = w * velocities[i][j] + c1 * (double) rand() / RAND_MAX * (individual_bests_pos[i][j] - population[i][j]) + c2 * (double) rand() / RAND_MAX * (global_best[j] - population[i][j]);
        }
    }
}

void PSO::updatePosition() {
    for (int i = 0; i < population_size; i++) {
        for (int j = 0; j < dimension; j++) {
            population[i][j] += velocities[i][j];
            if (population[i][j] > 1)
                population[i][j] = 1;
            if (population[i][j] < 0)
                population[i][j] = 0;
        }
    }
}

void PSO::evaluatePopulation() {
    // Calculate objective values
    for (int i = 0; i < population_size; i++) {
        vector<int> path = convertToPath(population[i], cities_normalized);
        objective_values[i] = evaluate(path, distances_normalized);
    }
    // Update global best
    for (int i = 0; i < population_size; i++) {
        if (objective_values[i] < bestFitness) {
            bestFitness = objective_values[i];
            global_best = population[i];
            best = convertToPath(population[i], cities_normalized);
            bestScore = evaluate(best);

            // Draw animation
            // string filename = "./output/file_" + to_string(++animation_file_count);
            // FILE *f = fopen(filename.c_str(), "w");
            // for (int i = 0; i < global_best.size(); i++) {
            //     fprintf(f, "%f ", global_best[i]);
            //     if (i & 1)
            //         fprintf(f, "\n");

            // }
            // fclose(f);
        }
    }
    // Update individual best
    for (int i = 0; i < population_size; i++) {
        if (objective_values[i] < individual_bests[i]) {
            individual_bests[i] = objective_values[i];
            individual_bests_pos[i] = population[i];
        }
    }
}

vector<double> PSO::run(int iterations) {
    for (int iter = 1; iter <= iterations; iter++) {
        evaluatePopulation();
        updateVelocity();
        updatePosition();
        // Record and log
        result.push_back(bestScore);
        if (iter % 100 == 0) {
            cout << "Iteration: " << iter << " Best fitness: " << bestFitness << " Best score: " << bestScore << endl;
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
    for (int i = 0; i < global_best.size(); i++) {
        cout << global_best[i] << " ";
        if (i & 1)
            cout << i/2 << endl;
    }
    cout << endl;
    cout << "Distance: " << bestScore;
    if (bestScore < 480) {
        cout << " XD";
    }
    cout << endl;
    return result;
}
