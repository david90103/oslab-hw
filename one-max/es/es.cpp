#include "es.h"

ES::ES(int bits, const char *seedfile) {
    char c;
    FILE *fp = fopen(seedfile, "r");
    score = 0;
    bestScore = 0;
    size = bits;
    arr = (bool *) malloc(sizeof(bool) * size);
    best = (bool *) malloc(sizeof(bool) * size);
    for (int i = 0; i < size; i++) {
        if (fp) {
            fscanf(fp, "%c", &c);
            arr[i] = best[i] = (c == '0') ? false : true;
        } else {
            arr[i] = best[i] =false;
        }
    }
    if (fp) {
        fclose(fp);
    }
}

void ES::printArray() {
    for (int i = 0; i < size; i++) {
        cout << (arr[i]) ? "1" : "0";
    }
    cout << " Score: " << score << " Best: " << bestScore << endl;
}

/**
 * Objective function
 */
int ES::eval() {
    int result = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i]) {
            result++;
        }
    }
    if (result > bestScore) {
        // Apply best value and array
        bestScore = result;
        for (int i = 0; i < size; i++) {
            best[i] = arr[i];
        }
    }
    return result;
}

/**
 * Get next enum by adding 1 to array
 */
void ES::nextEnum() {
    bool temp;
    int i = size - 1;
    bool carry = arr[i] & 1;
    arr[i] = !arr[i];
    while (carry && i > 0) {
        i--;
        temp = arr[i];
        arr[i] = carry ^ temp;
        carry = carry & temp;
    }
    score = eval();
}

vector<int> ES::run() {
    unsigned long long i = 0;
    while (!done()) {
        nextEnum();
        // printArray();
        // result.push_back(bestScore);
        if (i % 10000000 == 0) {
            cout << "Iteration: " << i << " Best score: " << bestScore << endl;
        } 
        i++;
    }
    cout << "Done." << endl;
    return result;
}
