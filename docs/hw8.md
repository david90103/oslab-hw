---
tags: oslab, HW
---

# 作業八

---

## 實作平行化GA在TSP

### 執行過程
```bash
$ cd tsp
$ make
$ ./main gap 30 5000 "data/eil51.tsp" 4 100 0.8 0.1 ox 
$ # 30 run, 5000 iter, 4 populations 100 chromosomes each population, 0.8 crossover, 0.1 mutation, order crossover
```

### 找到最佳解 (30 run, each run 5000 iterations)

- 找到最佳解：450
  - 0 > 47 > 22 > 23 > 42 > 6 > 25 > 7 > 30 > 27 > 2 > 35 > 34 > 19 > 28 > 20 > 15 > 49 > 33 > 29 > 8 > 48 > 9 > 38 > 32 > 44 > 14 > 36 > 4 > 37 > 10 > 1 > 21 > 31 > 50 > 45 > 11 > 46 > 17 > 3 > 16 > 43 > 41 > 18 > 39 > 40 > 12 > 24 > 13 > 5 > 26 > 0
- 平均最佳解：477.033333
- 時間：251 seconds

![路徑圖](https://i.imgur.com/VHgL0Pl.png)

### 平行化GA作法

- 產生多個population獨立演化，每過***10***個generation進行一次交換
- 交換過程：
    - 首先每個population隨機從目前所有解中挑選一個丟入交換池
    - 所有population都丟入後，每個population再從池中隨機選擇一個加入自己

### 收斂圖

平行化GA與一般GA的收斂曲線，可以看到前期一般GA成績較好但在1500 generations後被平行化GA超過去了(為了保持公平，一般GA使用 100 population size，平行GA使用8個populations 及 12 population size)

![收斂圖](https://i.imgur.com/HKoBOX6.png)

### 平行化GA參數比較

測試在使用相同總chromosome數量(populations * population size)的情況下，不同population數量與population大小對應的結果

![收斂比較圖](https://i.imgur.com/8OIIiYX.png)

收斂圖顯示調整平行化的參數會影響收斂速度，但是對最後的結果影響不大，不過使用多一點的population並讓每個population小一點可以讓收斂在超過幾千generations後還是有稍微下降的趨勢

### 程式說明

- Class GAP (GA Parallel) 繼承 Class GA，並加入一些平行化會需要用到的變數及function
```cpp=
class GAP : public GA {
private:
    // Pass a solution to another population every EXCHANGE_SOLUTION_GEN iterations
    static const int EXCHANGE_SOLUTION_GEN = 10;
    int total_threads;
    // Shared in all threads
    static int ready_to_exchange;
    static int current_exchange_gen;
    static int threads_to_wait;
    static vector<vector<int>> exchange_pool;
    // Lock for ready_to_exchange and exchange_pool
    static mutex exchange_lock;
    // Lock for bestScore and best
    static mutex best_score_lock;
    // Lock for result vector
    static mutex result_lock;
    // Override evalPopulation because we need to lock bestScore
    vector<double> evalPopulation(vector<bool> is_new_member, vector<vector<int>> pop, vector<double> f_values);
    vector<vector<int>> exchange(vector<vector<int>> population, int generation);
    void threadHandler(int generations);
略
```

- Override GA 的 evaluate function，因為要加入鎖機制避免多個thread搶著更新best fitness及best solution等變數
```cpp=
vector<double> GAP::evalPopulation(vector<bool> is_new_member, vector<vector<int>> pop, vector<double> f_values) {
    double temp;
    for (int i = 0; i < pop.size(); i++) {
        temp = fitness(pop[i]);
        f_values[i] = temp;
        if (f_values[i] < bestScore) {
            // Lock the mutex for variables before update them
            best_score_lock.lock();
            bestScore = f_values[i];
            best = pop[i];
            best_score_lock.unlock();
        }
    }
    return f_values;
}
```

- 每過特定generations，進入交換階段
```cpp=
// Exchange solution with other populations
if (gen % EXCHANGE_SOLUTION_GEN == 0)
    new_population = exchange(new_population, gen);
```

- 交換的過程
    - 剛進入此階段會檢查thread自己進行到的generation，因為有些thread會跑比較快，不希望20 generation的thread跟10 generation的thread進行交換
    - 檢查過後將thread中隨機的解丟入交換池中
    - 因為thread速度不同，需等待其他thread丟出他們的解
    - 最後隨機的從交換池中選擇一個加入自己population中
```cpp=
vector<vector<int>> GAP::exchange(vector<vector<int>> population, int generation) {
    // Wait for the generation for exchange mathces the generation this thread is running
    // This can prevent only one thread keeps getting the chance to finish whole exchange process
    while (generation > current_exchange_gen)
        this_thread::yield();
    
    int exchange_position = rand() % population.size();
    vector<int> s = population[exchange_position];
    // Lock the mutex and put solution to pool
    exchange_lock.lock();
    exchange_pool.push_back(s);
    ready_to_exchange++;
    threads_to_wait = total_threads;
    exchange_lock.unlock();

    // Wait for other threads to push their solution to exchange_pool
    while (ready_to_exchange < threads_to_wait)
        this_thread::yield();

    // Pick a solution from pool
    exchange_lock.lock();
    int r = rand() % exchange_pool.size();
    s = exchange_pool[r];
    exchange_pool.erase(exchange_pool.begin() + r);
    threads_to_wait = 0;
    ready_to_exchange--;
    if (ready_to_exchange == 0)
        current_exchange_gen = generation + EXCHANGE_SOLUTION_GEN + 1;
    exchange_lock.unlock();

    population[exchange_position] = s;
    return population;
}
```
---
