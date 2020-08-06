#include "./pso.h"

PSO::PSO(unsigned int randseed, int dimension, int population_size, double w, double c1, double c2, char const *seedfile) {
    srand(randseed);
    this->bestScore = 999.9;
    this->population_size = population_size;
    this->w = w;
    this->c1 = c1;
    this->c2 = c2;
    this->dimension = dimension;
    objective_values = vector<double>(population_size, 0.0);
    individual_bests = vector<double>(population_size, 999.9);
    // TODO: Seedfile
    // Initialize population
    vector<double> t;
    vector<double> temp(dimension, 0);
    for (int i = 0; i < population_size; i++) {
        t.clear();
        for (int j = 0; j < dimension; j++) {
            t.push_back(-INTERVAL_RANGE + (double) rand() / RAND_MAX * (2 * INTERVAL_RANGE));
        }
        population.push_back(t);
        individual_bests_pos.push_back(temp);
        velocities.push_back(temp);
    }
}

/**
 * Evaluate target with Ackley Function
 * http://www.sfu.ca/~ssurjano/ackley.html
 */
double PSO::evaluate(vector<double> target) {
    double sum1 = 0, sum2 = 0, exp1, exp2;

    for (int i = 0; i < dimension; i++) {
        sum1 += target[i] * target[i];
        sum2 += cos(2 * M_PI * target[i]);
    }
    exp1 = exp((-0.2) * sqrt(sum1 / dimension));
    exp2 = exp(sum2 / dimension);
    // Add 20 before exp2 to get accurate 0
    return -20 * exp1 + 20 - exp2 + exp(1);
}

void PSO::updateVelocity() {
    for (int i = 0; i < population_size; i++) {
        for (int j = 0; j < dimension; j++) {
            velocities[i][j] = w * population[i][j] + c1 * (double) rand() / RAND_MAX * (individual_bests_pos[i][j] - population[i][j]) + c2 * (double) rand() / RAND_MAX * (global_best[j] - population[i][j]);
        }
    }
}

void PSO::updatePosition() {
    for (int i = 0; i < population_size; i++) {
        for (int j = 0; j < dimension; j++) {
            population[i][j] += velocities[i][j];
        }
    }
}

void PSO::evaluatePopulation() {
    // Calculate objective values
    for (int i = 0; i < population_size; i++) {
        objective_values[i] = evaluate(population[i]);
    }
    // Update global best
    for (int i = 0;i < population_size; i++) {
        if (objective_values[i] < bestScore) {
            bestScore = objective_values[i];
            global_best = population[i];
        }
    }
    // Update individual best
    for (int i = 0; i < population_size; i++) {
        if (objective_values[i] < individual_bests[i]) {
            individual_bests[i] = objective_values[i];
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
        // if (iter % 10 == 0) {
        //     cout << "Iteration: " << iter << " Best score: " << bestScore << endl;
        // }
    }

    // for (int i = 0; i < dimension; i++) {
    //     cout << global_best[i] << " ";
    // }
    cout << "Score: " << bestScore << endl;
    return result;
}
