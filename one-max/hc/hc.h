#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "../lib/OneMax.h"
using namespace std;

/**
 * Hill climbing
 */
class HC : OneMax {
    private:
        int eval(bool *arr);
        bool isZero();

    public:
        HC(int bits, char const *seedfile);
        ~HC();
        void printArray();
        void nextEnum();
        int getBestScore() {return bestScore;}
        bool done() {return bestScore == size;}
};