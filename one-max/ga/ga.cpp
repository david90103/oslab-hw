#include "ga.h"

GA::GA(int bits, char const *seedfile) {
    char c;
    bool tmp;
    FILE *fp = fopen(seedfile, "r");
    srand(time(NULL));
    score = 0;
    bestScore = 0;
    size = bits;
    for (int i = 0; i < size; i++) {
        if (fp) {
            fscanf(fp, "%c", &c);
            tmp = (c == '0') ? false : true;
        } else {
            tmp = rand() & 1; 
        }
        arr.push_back(tmp);
        best.push_back(tmp);
    }
    if (fp) {
        fclose(fp);
    }
}

void GA::printArray() {
    for (int i = 0; i < arr.size(); i++) {
        cout << (arr[i]) ? "1" : "0";
    }
    cout << " Score: " << score << " Best: " << bestScore << endl;
}

int GA::getBestScore() {
    return bestScore;
}

/**
 * Objective function
 */
int GA::evaluate(vector<bool> arr) {
    int result = 0;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i]) {
            result++;
        }
    }
    return result;
}

/**
 * Get next enum by adding 1 or minus 1
 */
vector<bool> GA::transition() {
    bool temp;
    bool carry;
    int new_score;
    vector<bool> new_arr(arr);
    int i = new_arr.size() - 1;

    i = rand() % new_arr.size();
    new_arr[i] = !new_arr[i];
    return new_arr;
}

bool GA::isZero() {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == true) {
            return false;
        }
    }
    return true;
}

vector<int> GA::run(int iterations) {
    int new_enum_eval;
    vector<bool> new_enum;
    unsigned long long iter = 0;

    for (int iter = 1; iter <= iterations; iter++) {
        new_enum = transition();
        new_enum_eval = evaluate(new_enum);
        if (new_enum_eval > bestScore) {
            best = new_enum;
            bestScore = new_enum_eval;
        }
        if (new_enum_eval >= score) {
            arr = new_enum;
            score = new_enum_eval;
        }

        // Record and log
        result.push_back(bestScore);
        if (iter % 10 == 0) {
            cout << "Iteration: " << iter << " Best score: " << bestScore << endl;
        }
    }
    cout << "Done." << endl;
    return result;
}