#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

/**
 * Genetic Algorithm
 */
class GA {
    private:
        int size;
        int score;
        int bestScore;
        vector<bool> arr;
        vector<bool> best;
        vector<int> result;
        vector<bool> transition();
        bool isZero();
        int evaluate(vector<bool> arr);
    public:
        GA(int bits, char const *seedfile);
        void printArray();
        int getBestScore();
        vector<int> run(int iterations);
};