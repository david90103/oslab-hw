#include "sa.h"
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
    string filename;

    // Get output file name
    // do {
    //     file_index++;
    //     filename = "output_sa_run_" + std::to_string(file_index) + ".txt";
    // } while (exist(filename));
    // fp = fopen(filename.c_str(), "w+");

    SA sa = SA(5, 10.0, 0.0001, (char*) "");
    sa.run(10000000);
    
    return 0;
}