#include "./pso.h"

PSO::PSO(unsigned int randseed, int population_size, double w, double c1, double c2, char const *seedfile) {
    srand(randseed);
    this->bestScore = 999.9;
    this->population_size = population_size;
    this->w = w;
    this->c1 = c1;
    this->c2 = c2; 
    objective_values = vector<double>(population_size, 0.0);
    individual_bests = vector<double>(population_size, 999.9);
    // Initialize population
    for (int i = 0; i < population_size; i++) {
        population.push_back({MIN_X + (double) rand() / RAND_MAX * (MIN_X - MIN_X), MIN_Y + (double) rand() / RAND_MAX * (MIN_Y - MIN_Y)});
    }
}

/**
 * Evaluate target with Ackley Function
 * http://www.sfu.ca/~ssurjano/ackley.html
 */
double PSO::evaluate(vector<double> target) {
    /**
     * if x2:
            x = np.array([x1, x2])
        else:
            x = x1
        d = 2

        exp1 = np.exp((-0.2) * np.sqrt(np.sum(x**2) / d))
        exp2 = np.exp(np.sum(np.cos(2 * np.pi * x)) / d)

        return -20 * exp1 - exp2 + 20 + np.exp(1)
    */
}

void PSO::updateVelocity() {

}

void PSO::updatePosition() {
    for (int i = 0; i < population_size; i++) {
        
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
        if (iter % 10 == 0) {
            cout << "Iteration: " << iter << " Best score: " << bestScore << endl;
        }
    }
    return result;
}
