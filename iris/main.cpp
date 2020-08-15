#include "./ga/ga.hpp"
#include "./kmeans/kmeans.hpp"
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
    int iterations = 100;
    int clusters = 3;
    string seedfile = "";

    // GA parameters
    int encode_type = 0;
    int population_size = 10;
    double crossover_rate = 0.6;
    double mutation_rate = 0.01;

    /**
     * Parameters:
     * ga [runs] [iterations] [clusters] [seedfile] [encode type] [population size] [crossover rate] [mutation rate]
     * kmeans [runs] [iterations] [k] [seedfile]
     */
    algorithm = argv[1];
    runs = atoi(argv[2]);
    iterations = atoi(argv[3]);
    clusters = atoi(argv[4]);
    seedfile = argv[5];

    if (strcmp(algorithm.c_str(), "ga") == 0) {
        encode_type = atoi(argv[6]);
        population_size = atoi(argv[7]);
        crossover_rate = atof(argv[8]);
        mutation_rate = atof(argv[9]);
        for (int run = 0; run < runs; run++) {
            if (encode_type == 0) {
                ClusterIdGA ga = ClusterIdGA(time(NULL) + run, clusters, encode_type, population_size, crossover_rate, mutation_rate, seedfile.c_str());
                results.push_back(ga.run(iterations));
            }
            if (encode_type == 1) {
                CentroidsGA ga = CentroidsGA(time(NULL) + run, clusters, encode_type, population_size, crossover_rate, mutation_rate, seedfile.c_str());
                results.push_back(ga.run(iterations));
            }
            cout << "RUN " << run + 1 << " Done." << endl;
        }
    }
    if (strcmp(algorithm.c_str(), "kmeans") == 0) {
        for (int run = 0; run < runs; run++) {
            Kmeans km = Kmeans(time(NULL) + run, clusters, seedfile.c_str());
            results.push_back(km.run(iterations));
            cout << "RUN " << run + 1 << " Done." << endl;
        }
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
        filename = "output_" + algorithm + "_" + std::to_string(file_index) + ".txt";
    } while (exist(filename));
    fp = fopen(filename.c_str(), "w+");
    for (int i = 0; i < avg.size(); i++) {
        fprintf(fp, "%d %.20f\n", i + 1, avg[i]);
    }
    fclose(fp);

    return 0;
}