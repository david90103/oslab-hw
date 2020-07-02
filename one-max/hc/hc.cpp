#include "hc.h"

HC::HC(int iterations, int bits, char const *seedfile) {
    srand(time(NULL));
    this->arr = (char *) malloc(sizeof(char) * bits);
    this->best = (char *) malloc(sizeof(char) * bits);
    for (int i = 0; i < bits; i++) {
        this->arr[i] = rand() % 2 + '0';
        this->best[i] = this->arr[i];
        cout << this->arr[i];
    }
}

void HC::nextEnum() {
    // if (rand() % 2) {
    //     right();
    // } else {
    //     left();
    // }
}