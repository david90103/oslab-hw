---
tags: oslab, HW
---

# 作業六

---

### 實作DE在Ackley Function

#### 執行過程
```bash
$ cd ackley
$ make
$ ./search.sh # 30 run, 100 iter, dimension 2, size 10, crossover rate 0.7, f 0.6
```

#### 找到最佳解 (30 run, each run 200 iterations)

- 找到最佳解：0
- 30run最佳解之平均：0.00004670232785293215

#### 程式說明

- Ackley Funciton
```cpp
/**
 * Evaluate target with Ackley Function
 * http://www.sfu.ca/~ssurjano/ackley.html
 */
double DE::evaluate(vector<double> target) {
    double sum1 = 0, sum2 = 0, exp1, exp2;

    for (int i = 0; i < dimension; i++) {
        sum1 += target[i] * target[i];
        sum2 += cos(2 * M_PI * target[i]);
    }
    exp1 = exp((-0.2) * sqrt(sum1 / dimension));
    exp2 = exp(sum2 / dimension);
    // Add 20 before exp2 to get accurate 0
    return -20 * exp1 + 20 - exp2 + exp(1);
}
```

- 每個iteration重複mutation, crossover及selection
```cpp
for (int iter = 1; iter <= iterations; iter++) {
    vector<vector<double>> v_arr = mutation(population);
    vector<vector<double>> u_arr = crossover(population, v_arr);
    // Selection
    for (int i = 0; i < population_size; i++) {
        double eval = evaluate(u_arr[i]);
        if (eval <= objective_values[i]) {
            objective_values[i] = eval;
            population[i] = u_arr[i];
        }
        if (eval < bestScore) {
            bestScore = eval;
            global_best = u_arr[i];
        }
    }
}
```

- Mutation產生v陣列
```cpp
vector<vector<double>> DE::mutation(vector<vector<double>> population) {
    vector<vector<double>> v_arr;
    for (int i = 0; i < population.size(); i++) {
        vector<double> temp;
        int r1 = rand() % population_size;
        int r2 = rand() % population_size;
        int r3 = rand() % population_size;
        for (int j = 0; j < dimension; j++) {
            temp.push_back(population[r1][j] + f * (population[r2][j] - population[r3][j]));
        }
        v_arr.push_back(temp);
    }
    return v_arr;
}
```

- 使用原本的x及mutation的結果v進行crossover，產生u陣列
```cpp
vector<vector<double>> DE::crossover(vector<vector<double>> population, vector<vector<double>> v_arr) {
    vector<vector<double>> u_arr;
    for (int i = 0; i < population.size(); i++) {
        vector<double> temp;
        for (int j = 0; j < dimension; j++) {
            if (i == j || (double) rand() / RAND_MAX <= crossover_rate) {
                temp.push_back(v_arr[i][j]);
            } else {
                temp.push_back(population[i][j]);
            }
        }
        u_arr.push_back(temp);
    }
    return u_arr;
}
```

- Selection
```cpp
// Selection
for (int i = 0; i < population_size; i++) {
    double eval = evaluate(u_arr[i]);
    if (eval <= objective_values[i]) {
        objective_values[i] = eval;
        population[i] = u_arr[i];
    }
    if (eval < bestScore) {
        bestScore = eval;
        global_best = u_arr[i];
    }
}
```

---
