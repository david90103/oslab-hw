#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

/**
 * Exhausted search
 */
class ES {
    private:
        bool *arr;
        bool *best;
        int size;
        int score;
        int bestScore;
        void applyBest();
        int eval();

    public:
        ES(int bits, char const *seedfile);
        ~ES();
        void printArray();
        void nextEnum();
        int getBestScore() {return bestScore;}
        bool done() {return bestScore == size;}
};