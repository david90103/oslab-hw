#include "es.h"

int main(int argc, char *argv[]) {
    unsigned long long i = 0;

    ES es = ES(atoi(argv[1]), argv[2]);

    while (!es.done()) {
        es.nextEnum();
        // es.printArray();
        if (i % 10000000 == 0) {
            cout << "Iteration: " << i << " Best score: " << es.getBestScore() << endl;
        } 
        i++;
    }
    
    es.printArray();
    cout << "Done." << endl;

    return 0;
}