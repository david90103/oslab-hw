#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <iostream>
#include "../lib/OneMax.h"
using namespace std;

/**
 * Hill climbing
 */
class HC : public OneMax {
    private:
        int eval(bool *arr);
        bool isZero();

    public:
        HC(int bits, char *seedfile);
        void printArray() override;
        void nextEnum() override;
};