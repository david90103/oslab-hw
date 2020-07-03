#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "../lib/OneMax.h"
using namespace std;

/**
 * Exhausted search
 */
class ES : public OneMax {
    private:
        int eval();

    public:
        ES(int bits, char const *seedfile);
        void printArray() override;
        void nextEnum() override;
};