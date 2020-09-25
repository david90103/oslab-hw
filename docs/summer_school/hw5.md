---
tags: oslab, HW
---

# 作業五

---

### 實作PSO在Ackley Function

#### 執行過程
```bash
$ cd ackley
$ make
$ ./search.sh # 30 run, 100 iter, size 10, w 0.01, c1 0.7, c2 0.4
```

#### 找到最佳解 (30 run, each run 100 iterations or 1000 evaluations)

- 找到最佳解：4.44089e-16
- 30run最佳解之平均：4.44089e-16

#### 程式說明

- Ackley Function
```cpp
/**
 * Evaluate target with Ackley Function
 * http://www.sfu.ca/~ssurjano/ackley.html
 */
double PSO::evaluate(vector<double> target) {
    double sum1 = 0, sum2 = 0, exp1, exp2;
    
    for (int i = 0; i < DIMENSION; i++) {
        sum1 += pow(target[i], 2);
        sum2 += cos(2 * M_PI * target[i]);
    }
    exp1 = exp((-0.2) * sqrt(sum1 / DIMENSION));
    exp2 = exp(sum2 / DIMENSION);

    return -20 * exp1 - exp2 + 20 + exp(1);
}
```

- 每個iteration重複計算population的objective value，然後更新velocity以及position
```cpp
for (int iter = 1; iter <= iterations; iter++) {
    evaluatePopulation();
    updateVelocity();
    updatePosition();
    // Record and log
    result.push_back(bestScore);
}
```

- 更新velocity
```cpp
void PSO::updateVelocity() {
    for (int i = 0; i < population_size; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            velocities[i][j] = 
                w * population[i][j] +
                c1 * (double) rand() / RAND_MAX * (individual_bests_pos[i][j] - population[i][j]) + 
                c2 * (double) rand() / RAND_MAX * (global_best[j] - population[i][j]);
        }
    }
}
```

- 更新position
```cpp
void PSO::updatePosition() {
    for (int i = 0; i < population_size; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            population[i][j] += velocities[i][j];
        }
    }
}
```

---
