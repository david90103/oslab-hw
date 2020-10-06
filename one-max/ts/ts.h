#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

/**
 * Tabu Search
 */
class TS {
    private:
        int size;
        int score;
        int bestScore;
        int tabu_list_size;
        deque<vector<bool>> tabu_list;
        vector<bool> arr;
        vector<bool> best;
        vector<int> result;
        vector<bool> transition();
        vector<bool> transitionOnlyNeighbor();
        vector<bool> getNotTabuLeft(vector<bool> arr);
        vector<bool> getNotTabuRight(vector<bool> arr);
        vector<bool> getNotTabuRandom(vector<bool> arr);
        bool isZero(vector<bool>);
        bool isMax(vector<bool>);
        int evaluate(vector<bool> arr);
        inline bool inTabuList(vector<bool> target);
    public:
        TS(int bits, int tabu_list_size, char const *seedfile);
        void printArray();
        int getBestScore();
        vector<int> run(int iterations);
};