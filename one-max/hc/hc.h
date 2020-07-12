#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <iostream>
#include <vector>
#include "../lib/OneMax.h"
using namespace std;

/**
 * Hill climbing
 */
class HC : public OneMax {
    private:
        int eval(bool *arr);
        bool isZero();
        vector<int> result;
    public:
        HC(int bits, const char *seedfile);
        void printArray() override;
        void nextEnum() override;
        vector<int> run(int iterations);
};