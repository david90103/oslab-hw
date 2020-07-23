#include "aco.h"

ACO::ACO(int bits, int ants, double a, double b, double r, double q, char const *seedfile) {
    string line;
    srand(time(NULL));
    bestScore = 0;
    this->size = bits;
    this->ants = ants;
    this->alpha = a;
    this->beta = b;
    this->rho = r;
    this->q = q;
    fitness_values_sum = 0;
    // fitness_values = vector<int>(population_size, 0);
    
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
}

vector<int> ACO::run(int iterations) {
    for (int iter = 1; iter <= iterations; iter++) {
        
        // Record and log
        if (iter % 10 == 0) {
            cout << "Generation: " << iter << " Best score: " << bestScore << endl;
        }
    }
    
    cout << "Done." << endl;
    return vector<int>();
}