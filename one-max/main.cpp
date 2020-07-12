#include "./sa/sa.h"
#include "./ts/ts.h"
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

    // SA parameters
    double max_t = 10.0;
    double min_t = 0.0001;

    // TS parameters
    int tabu_list_size = 7;

    /**
     * Parameters:
     * es [runs] [iterations] [bits] [seedfile]
     * hc [runs] [iterations] [bits] [seedfile]
     * sa [runs] [iterations] [bits] [seedfile] [max temp] [min temp]
     * ts [runs] [iterations] [bits] [seedfile] [tabu list size]
     */
    algorithm = argv[1];
    runs = atoi(argv[2]);
    iterations = atoi(argv[3]);
    bits = atoi(argv[4]);
    seedfile = argv[5];

    if (strcmp(algorithm.c_str(), "es") == 0) {
        ///
    }
    if (strcmp(algorithm.c_str(), "hc") == 0) {
        ///
    }
    if (strcmp(algorithm.c_str(), "sa") == 0) {
        max_t = atof(argv[6]);
        min_t = atof(argv[7]);
        for (int run = 0; run < runs; run++) {
            SA sa = SA(bits, max_t, min_t, seedfile.c_str());
            results.push_back(sa.run(iterations));
            delay();
        }
    }
    if (strcmp(algorithm.c_str(), "ts") == 0) {
        tabu_list_size = atoi(argv[6]);
        for (int run = 0; run < runs; run++) {
            TS ts = TS(bits, tabu_list_size, seedfile.c_str());
            results.push_back(ts.run(iterations));
            delay();
        }
    }

    // Process results
    avg = vector<double>(iterations, 0);
    for (int i = 0; i < results.size(); i++) {
        for (int j = 0; j < results[i].size(); j++) {
            avg[j] += results[i][j];
        }
    }
    for (int i = 0; i < avg.size(); i++) {
        avg[i] /= results.size();
    }

    // Output to file
    do {
        file_index++;
        filename = "output_" + algorithm +"_" + std::to_string(file_index) + ".txt";
    } while (exist(filename));
    fp = fopen(filename.c_str(), "w+");
    for (int i = 0; i < avg.size(); i++) {
        fprintf(fp, "%d %f\n", i + 1, avg[i]);
    }
    fclose(fp);

    return 0;
}