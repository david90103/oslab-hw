#include "es.h"
#include <stdio.h>
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
    unsigned long long i = 1;
    string filename;

    // Get output file name
    do {
        file_index++;
        filename = "output_es_run_" + std::to_string(file_index) + ".txt";
    } while (exist(filename));
    fp = fopen(filename.c_str(), "w+");

    ES es = ES(atoi(argv[2]), argv[3]);

    while (!es.done()) {
        es.nextEnum();
        es.printArray();
        fprintf(fp, "%d %d\n", i, es.getBestScore());
        if (i % 10000000 == 0) {
            cout << "Iteration: " << i << " Best score: " << es.getBestScore() << endl;
        } 
        i++;
    }
    
    es.printArray();
    cout << "Done." << endl;

    return 0;
}