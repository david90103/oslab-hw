#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

/**
 * Simulated Annealing
 */
class SA {
    private:
        int size;
        int score;
        int bestScore;
        double max_temp;
        double min_temp;
        double temperature;
        vector<bool> arr;
        vector<bool> best;
        vector<int> result;
        vector<bool> transition();
        vector<bool> transition_only_neighbor();
        bool isZero();
        int evaluate(vector<bool> arr);
        inline double acceptance(int current_eval, int new_eval);
    public:
        SA(int bits, double max_temp, double min_temp, char const *seedfile);
        void printArray();
        int getBestScore();
        vector<int> run(int iterations);
};