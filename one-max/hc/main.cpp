#include "hc.h"
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
    unsigned long long iter = 0;
    std::string filename;

    // Get output file name
    do {
        file_index++;
        filename = "output_hc_run_" + std::to_string(file_index) + ".txt";
    } while (exist(filename));
    fp = fopen(filename.c_str(), "w+");

    HC hc = HC(atoi(argv[3]), argv[4]);

    for (int iter = 1; iter <= atoi(argv[2]); iter++) {
        hc.nextEnum();
        // hc.printArray();
        fprintf(fp, "%d %d \n", iter, hc.getBestScore());
        if (iter % 10 == 0) {
            cout << "Iteration: " << iter << " Best score: " << hc.getBestScore() << endl;
        } 
    }
    
    cout << "Done." << endl;
    fclose(fp);

    return 0;
}