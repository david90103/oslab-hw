#include "hc.h"

int main(int argc, char *argv[]) {
    unsigned long long i = 0;

    HC hc = HC(atoi(argv[2]), argv[3]);

    for (int i = 0; i < atoi(argv[1]); i++) {
        hc.nextEnum();
        hc.printArray();
        // if (i % 10000 == 0) {
        //     cout << "Iteration: " << i << " Best score: " << hc.getBestScore() << endl;
        // } 
        i++;
    }
    
    // hc.printArray();
    cout << "Done." << endl;

    return 0;
}