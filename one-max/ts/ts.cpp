#include "ts.h"

TS::TS(int bits, int t_list_size, char const *seedfile) {
    char c;
    bool tmp;
    FILE *fp = fopen(seedfile, "r");
    srand(time(NULL));
    score = 0;
    bestScore = 0;
    size = bits;
    tabu_list_size = t_list_size;
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

void TS::printArray() {
    for (int i = 0; i < arr.size(); i++) {
        cout << (arr[i]) ? "1" : "0";
    }
    cout << " Score: " << score << " Best: " << bestScore << endl;
}

int TS::getBestScore() {
    return bestScore;
}

/**
 * Objective function
 */
int TS::evaluate(vector<bool> arr) {
    int result = 0;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i]) {
            result++;
        }
    }
    return result;
}

inline bool TS::inTabuList(vector<bool> target) {
    int flag;
    for (int i = 0; i < tabu_list.size(); i++) {
        flag = 0;
        for (int j = 0; j < target.size(); j++) {
            if (target[j] == tabu_list[i][j]) {
                flag++;
            }
        }
        if (flag == target.size()) {
            return true;
        }
    }
    return false;
}

vector<bool> TS::getNotTabuLeft(vector<bool> arr) {
    bool temp;
    bool carry;
    int i;
    if (isZero(arr)) {
        return arr;
    }
    do {
        i = arr.size() - 1;
        if (!arr[i]) {
            do {
                i--;
                temp = arr[i];
                arr[i] = true;
            } while (!temp);
            arr[i] = false;
        }
        arr[arr.size() - 1] = !arr[arr.size() - 1];
    } while (inTabuList(arr));
    return arr;
}

vector<bool> TS::getNotTabuRight(vector<bool> arr) {
    bool temp;
    bool carry;
    int i;
    if (isMax(arr)) {
        return arr;
    }
    do {
        i = arr.size() - 1;
        carry = arr[i] & 1;
        arr[i] = !arr[i];
        while (carry && i > 0) {
            i--;
            temp = arr[i];
            arr[i] = carry ^ temp;
            carry = carry & temp;
        }
    } while (inTabuList(arr));
    return arr;
}

vector<bool> TS::getNotTabuRandom(vector<bool> arr) {
    do {
        int i = rand() % arr.size();
        arr[i] = !arr[i];
    } while (inTabuList(arr));
    return arr;
}

/**
 * Get next enum by searching neighbor that is not in tabu list
 */
vector<bool> TS::transitionOnlyNeighbor() {
    if (rand() % 2 == 0) {
        return getNotTabuLeft(arr);
    }
    return getNotTabuRight(arr);
}

/**
 * Get next enum by random flip bit
 */
vector<bool> TS::transition() {
    return getNotTabuRandom(arr);
}

bool TS::isZero(vector<bool> arr) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == true) {
            return false;
        }
    }
    return true;
}

bool TS::isMax(vector<bool> arr) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] != true) {
            return false;
        }
    }
    return true;
}

vector<int> TS::run(int iterations) {
    unsigned long long iter = 0;
    int temp_score = 0;
    vector<bool> temp_solution;

    for (int iter = 1; iter <= iterations; iter++) {
        temp_solution = transition();
        temp_score = evaluate(temp_solution);
        if (temp_score > score) {
            arr = temp_solution;
            score = temp_score;
        }
        if (temp_score > bestScore) {
            best = arr;
            bestScore = temp_score;
        }
        // Update tabu list
        tabu_list.push_back(arr);
        if (tabu_list.size() > tabu_list_size) {
            tabu_list.pop_front();
        }
        // Print tabu list
        // cout << "Tabu list: ";
        // for (int i = 0; i < tabu_list.size(); i++) {
        //     for (int j = 0; j < tabu_list[i].size(); j++) {
        //         cout << (tabu_list[i][j]) ? "1" : "0";
        //     }
        //     cout << " ";
        // }
        // cout << endl;
        // printArray();
        // Record and log
        result.push_back(bestScore);
        if (iter % 10 == 0) {
            cout << "Iteration: " << iter << " Best score: " << bestScore << endl;
        }
    }
    cout << "Done." << endl;
    return result;
}