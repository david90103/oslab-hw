#include "pso1d.hpp"

PSO1D::PSO1D(unsigned int randseed, int population_size, double w, double c1, double c2, char const *seedfile) {
    srand(randseed);
    this->bestScore = DBL_MAX;
    this->bestFitness = DBL_MAX;
    this->population_size = population_size;
    this->w = w;
    this->c1 = c1;
    this->c2 = c2;
    this->cities = readCitiesFromFile(seedfile);
    this->cities_normalized = normalize(this->cities);
    this->dimension = cities.size();
    this->objective_values = vector<double>(population_size, DBL_MAX);
    this->individual_bests = vector<double>(population_size, DBL_MAX);
    this->distances = initDistances(this->cities);
    this->distances_normalized = initDistances(this->cities_normalized);
    this->animation_file_count = 0;

    // Initialize population
    vector<double> t;
    vector<double> temp(dimension, 0);
    for (int i = 0; i < population_size; i++) {
        t.clear();
        for (int j = 0; j < dimension; j++) {
            t.push_back((double) rand() / RAND_MAX);
        }
        population.push_back(t);
        individual_bests_pos.push_back(temp);
        t.clear();
        for (int j = 0; j < dimension; j++) {
            // t.push_back(-1 + (double) rand() / RAND_MAX * 2); // -1 ~ 1
            t.push_back((double) rand() / RAND_MAX); // 0 ~ 1
        }
        velocities.push_back(t);
    }
}

void PSO1D::updateVelocity() {
    for (int i = 0; i < population_size; i++) {
        for (int j = 0; j < dimension; j++) {
            velocities[i][j] = w * velocities[i][j] + c1 * (double) rand() / RAND_MAX * (individual_bests_pos[i][j] - population[i][j]) + c2 * (double) rand() / RAND_MAX * (global_best[j] - population[i][j]);
        }
    }
}

void PSO1D::updatePosition() {
    for (int i = 0; i < population_size; i++) {
        for (int j = 0; j < dimension; j++) {
            population[i][j] += velocities[i][j];
            // if (population[i][j] > 1)
            //     population[i][j] = 1;
            // if (population[i][j] < 0)
            //     population[i][j] = 0;
        }
    }
}

void PSO1D::evaluatePopulation() {
    // Calculate objective values
    for (int i = 0; i < population_size; i++) {
        // vector<int> path = convertToPathNormalized(population[i]);
        // objective_values[i] = evaluateNormalized(path);
        vector<int> path = convertToPathNormalized(population[i]);
        objective_values[i] = evaluateNormalized(path);
    }
    // Update global best
    for (int i = 0; i < population_size; i++) {
        if (objective_values[i] < bestFitness) {
            bestFitness = objective_values[i];
            global_best = population[i];
            best = convertToPathNormalized(population[i]);
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

vector<double> PSO1D::run(int iterations) {
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
        cout << i << endl;
    }
    cout << endl;
    cout << "Distance: " << bestScore;
    if (bestScore < 480) {
        cout << " XD";
    }
    cout << endl;
    return result;
}
