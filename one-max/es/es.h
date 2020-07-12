#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "../lib/OneMax.h"
using namespace std;

/**
 * Exhausted search
 */
class ES : public OneMax {
    private:
        int eval();
        vector<int> result;
    public:
        ES(int bits, const char *seedfile);
        void printArray() override;
        void nextEnum() override;
        vector<int> run();
};