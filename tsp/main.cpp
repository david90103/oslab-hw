#include "./aco/aco.h"
#include "./ga/ga.hpp"
#include "./ga/gap.hpp"
#include "./de/de.hpp"
#include "./de/de1d.hpp"
#include "./de/de1dc.hpp"
#include "./pso/pso.hpp"
#include "./pso/pso1d.hpp"
#include <stdio.h>
#include <time.h>
#include <string>

inline bool exist(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    FILE *fp;
    int file_index = 0;
    string filename;
    vector<vector<double>> results;
    vector<double> avg;

    string algorithm = "";
    int runs = 30;
    int bits = 100;
    int iterations = 100;
    string seedfile = "";

    // ACO parameters
    int ants = 20;
    double alpha = 1.0;
    double beta = 2.0;
    double rho = 0.1;
    double q = 1.0;

    int population_size = 20;

    // GA parameters
    double ga_crossover_rate = 0.7;
    double ga_mutation_rate = 0.1;
    string ga_crossover_method = "pmx";
    int populations = 4;
    
    // DE parameters
    double crossover_rate = 0.7;
    double f = 0.6;

    // PSO parameters
    double w = 1.0;
    double c1 = 1.0;
    double c2= 1.0;

    /**
     * Parameters:
     * aco [runs] [iterations] [seedfile] [ants] [alpha] [beta] [rho] [q]
     * ga  [runs] [iterations] [seedfile] [population size] [crossover rate] [mutation rate] [crossover method]
     * gap [runs] [iterations] [seedfile] [populations] [population size] [crossover rate] [mutation rate] [crossover method]
     * de  [runs] [iterations] [seedfile] [population size] [crossover rate] [f]
     * pso
     */
    algorithm = argv[1];
    runs = atoi(argv[2]);
    iterations = atoi(argv[3]);
    seedfile = argv[4];

    if (strcmp(algorithm.c_str(), "aco") == 0) {
        ants = atoi(argv[5]);
        alpha = atof(argv[6]);
        beta = atof(argv[7]);
        rho = atof(argv[8]);
        q = atof(argv[9]);
        time_t start = time(NULL);
        for (int run = 0; run < runs; run++) {
            ACO aco = ACO(time(NULL) + run, ants, alpha, beta, rho, q, seedfile.c_str());
            results.push_back(aco.run(iterations));
            cout << "RUN " << run + 1 << " Done." << endl;
        }
        cout << "Time: " << time(NULL) - start << endl;
    }

    if (strcmp(algorithm.c_str(), "ga") == 0) {
        population_size = atoi(argv[5]);
        ga_crossover_rate = atof(argv[6]);
        ga_mutation_rate = atof(argv[7]);
        ga_crossover_method = argv[8];
        time_t start = time(NULL);
        for (int run = 0; run < runs; run++) {
            GA ga = GA(time(NULL) + run, population_size, ga_crossover_rate, ga_mutation_rate, ga_crossover_method.c_str(), seedfile.c_str());
            results.push_back(ga.run(iterations));
            cout << "RUN " << run + 1 << " Done." << endl;
        }
        cout << "Time: " << time(NULL) - start << endl;
    }

    if (strcmp(algorithm.c_str(), "gap") == 0) {
        populations = atoi(argv[5]);
        population_size = atoi(argv[6]);
        ga_crossover_rate = atof(argv[7]);
        ga_mutation_rate = atof(argv[8]);
        ga_crossover_method = argv[9];
        time_t start = time(NULL);
        for (int run = 0; run < runs; run++) {
            GAP gap = GAP(time(NULL) + run, populations, population_size, ga_crossover_rate, ga_mutation_rate, ga_crossover_method.c_str(), seedfile.c_str());
            results.push_back(gap.run(iterations));
            cout << "RUN " << run + 1 << " Done." << endl;
        }
        cout << "Time: " << time(NULL) - start << endl;
    }

    if (strcmp(algorithm.c_str(), "de") == 0) {
        population_size = atoi(argv[5]);
        crossover_rate = atof(argv[6]);
        f = atof(argv[7]);
        time_t start = time(NULL);
        for (int run = 0; run < runs; run++) {
            DE de = DE(time(NULL) + run, population_size, crossover_rate, f, seedfile.c_str());
            results.push_back(de.run(iterations));
            cout << "RUN " << run + 1 << " Done." << endl;
        }
        cout << "Time: " << time(NULL) - start << endl;
    }

    if (strcmp(algorithm.c_str(), "de1d") == 0) {
        population_size = atoi(argv[5]);
        crossover_rate = atof(argv[6]);
        f = atof(argv[7]);
        time_t start = time(NULL);
        for (int run = 0; run < runs; run++) {
            DE1D de1d = DE1D(time(NULL) + run, population_size, crossover_rate, f, seedfile.c_str());
            results.push_back(de1d.run(iterations));
            cout << "RUN " << run + 1 << " Done." << endl;
        }
        cout << "Time: " << time(NULL) - start << endl;
    }

    if (strcmp(algorithm.c_str(), "de1dc") == 0) {
        population_size = atoi(argv[5]);
        crossover_rate = atof(argv[6]);
        f = atof(argv[7]);
        time_t start = time(NULL);
        for (int run = 0; run < runs; run++) {
            DE1DC de1dc = DE1DC(time(NULL) + run, population_size, crossover_rate, f, seedfile.c_str());
            results.push_back(de1dc.run(iterations));
            cout << "RUN " << run + 1 << " Done." << endl;
        }
        cout << "Time: " << time(NULL) - start << endl;
    }

    if (strcmp(algorithm.c_str(), "pso") == 0) {
        population_size = atoi(argv[5]);
        w = atof(argv[6]);
        c1 = atof(argv[7]);
        c2 = atof(argv[8]);
        time_t start = time(NULL);
        for (int run = 0; run < runs; run++) {
            PSO pso = PSO(time(NULL) + run, population_size, w, c1, c2, seedfile.c_str());
            results.push_back(pso.run(iterations));
            cout << "RUN " << run + 1 << " Done." << endl;
        }
        cout << "Time: " << time(NULL) - start << endl;
    }

    if (strcmp(algorithm.c_str(), "pso1d") == 0) {
        population_size = atoi(argv[5]);
        w = atof(argv[6]);
        c1 = atof(argv[7]);
        c2 = atof(argv[8]);
        time_t start = time(NULL);
        for (int run = 0; run < runs; run++) {
            PSO1D pso1d = PSO1D(time(NULL) + run, population_size, w, c1, c2, seedfile.c_str());
            results.push_back(pso1d.run(iterations));
            cout << "RUN " << run + 1 << " Done." << endl;
        }
        cout << "Time: " << time(NULL) - start << endl;
    }

    // Process results
    for (int i = 0; i < results.size(); i++) {
        for (int j = 0; j < results[i].size(); j++) {
            if (avg.size() < j + 1) {
                avg.push_back(0);
            }
            avg[j] += results[i][j];
        }
    }
    for (int i = 0; i < avg.size(); i++) {
        avg[i] /= results.size();
    }

    // Output to file
    do {
        file_index++;
        filename = "output_" + algorithm + "_" + to_string(population_size) + "_" + to_string(crossover_rate) + "_" + to_string(f) + "_" + std::to_string(file_index) + ".txt";
    } while (exist(filename));
    fp = fopen(filename.c_str(), "w+");
    for (int i = 0; i < avg.size(); i++) {
        fprintf(fp, "%d %f\n", i + 1, avg[i]);
    }
    fclose(fp);

    return 0;
}