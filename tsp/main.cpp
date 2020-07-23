#include "./aco/aco.h"
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

void delay() {
    time_t start = time(NULL);
    while (start == time(NULL))
        ;
}

int main(int argc, char *argv[]) {
    FILE *fp;
    int file_index = 0;
    string filename;
    vector<vector<int>> results;
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

    /**
     * Parameters:
     * aco [runs] [iterations] [seedfile] [ants] [alpha] [beta] [rho] [q]
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
        for (int run = 0; run < runs; run++) {
            ACO aco = ACO(ants, alpha, beta, rho, q, seedfile.c_str());
            results.push_back(aco.run(1000));
            delay();
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
        fprintf(fp, "%d %f\n", i + 1, avg[i]);
    }
    fclose(fp);

    return 0;
}