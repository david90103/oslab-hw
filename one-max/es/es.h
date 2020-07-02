#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "../lib/OneMax.h"
using namespace std;

/**
 * Exhausted search
 */
class ES : OneMax {
    private:
        int eval();

    public:
        ES(int bits, char const *seedfile);
        ~ES();
        void printArray();
        void nextEnum();
        int getBestScore() {return bestScore;}
        bool done() {return bestScore == size;}
};