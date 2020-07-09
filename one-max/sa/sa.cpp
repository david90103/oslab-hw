#include "sa.h"

SA::SA(int bits, double max_t, double min_t, char *seedfile) {
    char c;
    bool tmp;
    FILE *fp = fopen(seedfile, "r");
    srand(time(NULL));
    temperature = max_temp = max_t;
    min_temp = min_t;
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

void SA::printArray() {
    for (int i = 0; i < arr.size(); i++) {
        cout << (arr[i]) ? "1" : "0";
    }
    cout << " Score: " << score << " Best: " << bestScore << endl;
}

/**
 * Objective function
 */
int SA::evaluate(vector<bool> arr) {
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
vector<bool> SA::transition() {
    bool temp;
    bool carry;
    int new_score;
    vector<bool> new_arr(arr);
    int i = new_arr.size() - 1;

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
            new_arr[new_arr.size() - 1] = !new_arr[new_arr.size() - 1];
        }
    }
    return new_arr;
}

bool SA::isZero() {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == true) {
            return false;
        }
    }
    return true;
}


/**
 * Acceptance of SA
 * https://en.wikipedia.org/wiki/Simulated_annealing
 */
inline double SA::acceptance(int current_eval, int new_eval) {
    double t = exp(-((double) current_eval - new_eval) / temperature);
    // cout << (double) new_eval - current_eval << endl;
    return t;
}

void SA::run(int iterations) {
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
        } else {
            // cout << acceptance(score, new_enum_eval) << endl;
            if ((double) rand() / RAND_MAX < acceptance(score, new_enum_eval)) {
                arr = new_enum;
                score = new_enum_eval;
                cout << "accepted" << endl;
            } else {
                // cout << "not accepted" << endl;
            }
        }
        printArray();
        temperature *= 0.999;
        if (iter % 10 == 0) {
            cout << "Iteration: " << iter << " Best score: " << bestScore << endl;
        }
        if (temperature <= min_temp) {
            cout << "Reached min temperature!" << endl;
            break;
        }
    }
    cout << "Done." << endl;
}