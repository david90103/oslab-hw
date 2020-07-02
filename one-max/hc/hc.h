#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

class HC {
    private:
        char *arr;
        char *best;
        int bestScore;

    public:
        HC(int iterations, int bits, char const *seedfile);
        void nextEnum();
        // void left();
        // void right();
        int getBestScore() {return bestScore;}
};