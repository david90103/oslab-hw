---
tags: oslab, HW
---

# 作業三

---

### 實作GA在One-Max Problem

#### 執行過程
```bash
$ cd one-max
$ make
$ ./main ga 30 100 100 "" 100 0.6 0.01 # population size: 100, crossover rate: 0.6, mutation rate 0.01
```

#### 找到最佳解 (100 Generations)
- Roulette Wheel
    - 找到最佳解：79
    - 30run最佳解之平均：76.233
- Tournament Selection
    - 找到最佳解：96
    - 30run最佳解之平均：92.0

#### 程式說明

- evalPopulation用來計算每個chromosome的fitness
  - 參數會傳入一個is_new_member的vector，用來記錄population中的每個chromosome是由crossover所產生的還是直接由父代繼承過來
  - 如果一個chromosome是crossover的結果，要做紀錄(因為收斂圖x軸是number of evaluation)
```cpp
/**
 * Evaluate every chromosome in population
 * Parameter is_new_member is a boolean list to track if a chromosome
 * in population is created by crossover or directly from parents
 */
void GA::evalPopulation(vector<bool> is_new_member) {
    int temp;
    fitness_values_sum = 0;
    for (int i = 0; i < population.size(); i++) {
        temp = fitness(population[i]);
        fitness_values[i] = temp;
        fitness_values_sum += temp;
        if (fitness_values[i] > bestScore) {
            bestScore = fitness_values[i];
            best = population[i];
        }
        if (is_new_member[i]) {
            result.push_back(bestScore);
        }
    }
}
```

- Selection: Roulette Wheel
  - 輪盤法會考慮所有chromosome的fitness並依照好壞對應到被選擇的機率，因此較差的chromosome也是有機會被選中
  - 實作中利用fitness_values_sum儲存每次generation的fitness總和，優化輪盤法的速度
```cpp
vector<bool> GA::rouletteWheel() {
    int sum = 0;
    int target = rand() % fitness_values_sum;
    for (int i = 0; i < population_size; i++) {
        sum += fitness_values[i];
        if (sum >= target) {
            return population[i];
        }
    }
    return population[population_size - 1];
}
```

- Selection: Tournament Selection
  - 隨機比較兩個chromosome的fitness，並選擇fitness較高的
  - 收斂較快
  - 但會導致fitness較低的chromosome中一些比較好的基因無法延續下去

```cpp
vector<bool> GA::tournament() {
    int i = rand() % population_size;
    int j = rand() % population_size;
    if (fitness_values[i] >= fitness_values[j]) {
        return population[i];
    }
    return population[j];
}
```

- Crossover
  - 交配時，隨機選擇一個點作為切割雙親的pivot，交叉組合後會產生兩個子代
```cpp
vector<vector<bool>> GA::crossover(vector<bool> father, vector<bool> mother) {
    int pivot = rand() % father.size();
    vector<bool> child_a(father.begin(), father.begin() + pivot);
    vector<bool> child_b(mother.begin(), mother.begin() + pivot);
    child_a.insert(child_a.end(), mother.begin() + pivot, mother.end());
    child_b.insert(child_b.end(), father.begin() + pivot, father.end());
    return vector<vector<bool>> {child_a, child_b};
}
```

- Mutation

```cpp
inline vector<bool> GA::mutation(vector<bool> target) {
    int i = rand() % target.size();
    target[i] = !target[i];
    return target;
}
```


- 每個generation最後要將子代替換作父代並計算fitness value

```cpp
// Apply population and fitness values
population = new_population;
evalPopulation(new_member_list);
```

---