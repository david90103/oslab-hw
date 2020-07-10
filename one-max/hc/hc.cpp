#include "hc.h"

HC::HC(int bits, char *seedfile) {
    char c;
    FILE *fp = fopen(seedfile, "r");
    srand(time(NULL));
    bestScore = 0;
    size = bits;
    arr = (bool *) malloc(sizeof(bool) * size);
    best = (bool *) malloc(sizeof(bool) * size);
    for (int i = 0; i < size; i++) {
        if (fp) {
            fscanf(fp, "%c", &c);
            arr[i] = best[i] = (c == '0') ? false : true;
        } else {
            arr[i] = best[i] = rand() & 1;
        }
    }
    if (fp) {
        fclose(fp);
    }
}

void HC::printArray() {
    for (int i = 0; i < size; i++) {
        cout << (arr[i]) ? "1" : "0";
    }
    cout << " Score: " << score << " Best: " << bestScore << endl;
}

/**
 * Objective function
 */
int HC::eval(bool *arr) {
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
 * Get next enum by adding 1 or minus 1
 */
void HC::nextEnum() {
    bool temp;
    bool carry;
    int new_score;
    int i = size - 1;
    bool *new_arr = (bool *) malloc(sizeof(bool) * size);

    // copy arr
    for (int i = 0; i < size; i++) {
        new_arr[i] = arr[i];
    }

    if (rand() % 2 == 0) {
        // add 1
        carry = new_arr[i] & 1;
        new_arr[i] = !new_arr[i];
        while (carry && i > 0) {
            i--;
            temp = new_arr[i];
            new_arr[i] = carry ^ temp;
            carry = carry & temp;
        }
    } else {
        // minus 1
        if (!isZero()) {
            if (!new_arr[i]) {
                do {
                    i--;
                    temp = new_arr[i];
                    new_arr[i] = true;
                } while (!temp);
                new_arr[i] = false;
            }
            new_arr[size - 1] = !new_arr[size - 1];
        }
    }
    new_score = eval(new_arr);
    
    if (new_score >= score) {
        // apply new array
        score = new_score;
        for (int i = 0; i < size; i++) {
            arr[i] = new_arr[i];
        }
    }
    free(new_arr);
}

bool HC::isZero() {
    for (int i = 0; i < size; i++) {
        if (arr[i] == true) {
            return false;
        }
    }
    return true;
}