---
tags: oslab, HW
---

# 作業四

---

### 實作ACO在TSP(eil51)

#### 執行過程
```bash
$ cd tsp
$ make
$ ./search.sh # 30 run, 100 iter, 20 ants, alpha 0.9, beta 5.0, rho 0.1, Q 0.9
```

#### 找到最佳解 (30 run, each run 100 iterations or 2000 evaluations)

- 找到最佳解：428.8
- 30run最佳解之平均：436.3
- 執行時間：14秒

#### 程式說明

- 從檔案讀取城市資料並建構表格
```cpp
// Read cities from seedfile
ifstream fp(seedfile);
while (getline(fp, line)) {
    if (isdigit(line.front())) {
        vector<double> split;
        string pattern = " ";
        line += pattern;
        for (int i = 0; i < line.size(); i++) {
            int pos = (int) line.find(pattern, i);
            if (pos != string::npos) {
                split.push_back(atof(line.substr(i, pos - i).c_str()));
                i = pos + (int) pattern.size() - 1;
            }
        }
        split.erase(split.begin());
        cities.push_back(split);
    }
}
```

- 有了城市資料後建立距離表及費洛蒙表
  - 費洛蒙初始值設定為`0.00001`，太多太少都會有問題
```cpp
// aco.h
const double PHEROMONE_INIT = 0.00001;

// aco.cpp
// Initialize distance table
for (int i = 0; i < cities.size(); i++) {
    distances.push_back(vector<double>());
    for (int j = 0; j < cities.size(); j++) {
        distances[i].push_back(distance(cities[i], cities[j]));
    }
}
// Initialize pheromone table
vector<double> temp(cities.size(), PHEROMONE_INIT);
for (int j = 0; j < cities.size(); j++) {
    pheromone.push_back(temp);
}
```

- 每個iteration重複建構路徑及更新費洛蒙
```cpp
for (int iter = 1; iter <= iterations; iter++) {
    generatePath();
    updatePheromone();
}
```

- 建構路徑
  - 第一個城市以隨機產生，之後利用一個candidate_city紀錄目前還沒去過的城市，透過輪盤選出城市後把城市從candidate_city移除
```cpp
void ACO::generatePath() {
    int current, first;
    double score;
    vector<int> path;
    vector<int> candidate_city;
    population.clear();
    for (int i = 0; i < ants; i++) {
        path.clear();
        candidate_city.clear();
        current = rand() % cities.size();
        first = current;
        for (int j = 0; j < cities.size(); j++) {
            candidate_city.push_back(j);
        }
        for (int j = 0; j < cities.size(); j++) {
            path.push_back(current);
            // Remove current city
            for (vector<int>::iterator iter = candidate_city.begin(); iter != candidate_city.end(); iter++) {
                if(*iter == current) {
                    candidate_city.erase(iter);
                    break;
                }
            }
            if (candidate_city.size() > 0) {
                current = rouletteWheelwithRBP(current, candidate_city);
            }
        }
        path.push_back(first);
        population.push_back(path);
    }
    // Evaluate Population
    // ...
}
```

- Roulette Wheel
  - 根據ACO的方法產生選擇各路線的機率
  - 參數 alpha: 0.9, beta: 5.0
```cpp
for (int i = 0; i < candidate_city.size(); i++) {
    temp = pow(pheromone[current_city][candidate_city[i]], alpha) * pow(1 / distances[current_city][candidate_city[i]], beta);
    sum += temp;
    table.push_back(temp);
}
for (int i = 0; i < table.size(); i++) {
    table[i] = table[i] / sum;
}
target = (double) rand() / RAND_MAX;
sum = 0;
for (int i = 0; i < table.size(); i++) {
    sum += table[i];
    if (sum > target) {
        return candidate_city[i];
    }
}
```

- 更新費洛蒙表格
  - http://people.idsia.ch/~luca/acs-ec97.pdf
  - 看過上面ACO的論文後發現Ant Colony System在實作上有三種更新費洛蒙的方式
    - 一般的 pheromone update：螞蟻會更新所走過的路徑
    - Local pheromone update：將路徑都加上rho * 基本費洛蒙數值，避免所有的螞蟻都選擇同一條路徑
    - Global pheromone update：保留最佳解的路徑費洛蒙
  - 測試分別使用及不使用這三種更新方式對結果的影響，產生以下表格(1250 evaluations)

|   | 一般更新 | Local update | Global update | Result |
|---|----------|--------------|---------------|--------|
| 1 |     V    |              |               |   464  |
| 2 |          |       V      |               |   499  |
| 3 |          |              |       V       |   444  |
| 4 |     V    |       V      |               |   464  |
| 5 |     V    |              |       V       |   461  |
| 6 |          |       V      |       V       |   440  |
| 7 |     V    |       V      |       V       |   463  |

```cpp
void ACO::updatePheromone() {
    vector<vector<double>> add_pheromone;
    vector<double> temp(cities.size(), 0);
    for (int i = 0; i < cities.size(); i++) {
        add_pheromone.push_back(temp);
    }
    /**
     * Pheromone Update
     * TODO: Try to update pheromone during generating path
     */
    for (int i = 0; i < ants; i++) {
        for (int j = 0; j < population[i].size() - 1; j++) {
            // Sync a->b with b->a
            add_pheromone[population[i][j]][population[i][j + 1]] += q / fitness_values[i];
            add_pheromone[population[i][j + 1]][population[i][j]] += q / fitness_values[i];
        }
    }
    for (int i = 0; i < pheromone.size(); i++) {
        for (int j = 0; j < pheromone[i].size(); j++) {
            pheromone[i][j] = ((1 - rho) * pheromone[i][j]) + add_pheromone[i][j];
        }
    }
    /**
     * Local Pheromone Update
     * The main goal of the local update is to diversify the
     * search performed by subsequent ants during an iteration
     * https://courses.cs.ut.ee/all/MTAT.03.238/2011K/uploads/Main/04129846.pdf
     */
    for (int i = 0; i < pheromone.size(); i++) {
        for (int j = 0; j < pheromone[i].size(); j++) {
            pheromone[i][j] = ((1 - rho) * pheromone[i][j]) + (rho * PHEROMONE_INIT);
        }
    }
    /**
     * Global Pheromone Update
     * This update will keep the global best path information
     * http://people.idsia.ch/~luca/acs-ec97.pdf
     */
    for (int i = 0; i < add_pheromone.size(); i++) {
        for (int j = 0; j < add_pheromone[i].size(); j++) {
            add_pheromone[i][j] = 0;
        }
    }
    for (int i = 0; i < best.size() - 1; i++) {
        // Sync a->b with b->a
        add_pheromone[best[i]][best[i + 1]] += 1 / bestScore;
        add_pheromone[best[i + 1]][best[i]] += 1 / bestScore;
    }
    for (int i = 0; i < pheromone.size(); i++) {
        for (int j = 0; j < pheromone[i].size(); j++) {
            pheromone[i][j] = (1 - GLOBAL_SEARCH_ALPHA) * pheromone[i][j] + GLOBAL_SEARCH_ALPHA * add_pheromone[i][j];
        }
    }
}
```

---
