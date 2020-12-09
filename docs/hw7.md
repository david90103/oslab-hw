---
tags: oslab, HW
---

# 作業七

---

### 實作GA在TSP

#### 執行過程
```bash
$ cd tsp
$ make
$ ./search.sh # 30 run, 5000 iter, pop size 100, crossover 0.8, mutation 0.1, order crossover
```

#### 找到最佳解 (30 run, each run 5000 iterations)

- 找到最佳解：474

- Order crossover 
  - 30run最佳解之平均：534.200000
  
- Cycle crossover 
  - 30run最佳解之平均：604.300000

- Partially mapped crossover 
  - 30run最佳解之平均：582.200000

#### 程式說明

- 根據傳入的參數設定對應的crossover method
```cpp
// In constructor of GA class
if (strcmp(crossover_method, "pmx") == 0)
    crossover = &GA::partiallyMappedCrossover;
else if (strcmp(crossover_method, "cx") == 0)
    crossover = &GA::cycleCrossover;
else // ox
    crossover = &GA::orderCrossover;
```

- 初始化population，每條路徑都由城市0開始並回到城市0
```cpp
for (int i = 0; i < population_size; i++) {
    temp_path.clear();
    // Start from city 0
    temp_path.push_back(0);
    candidate_cities.clear();
    for (int j = 1; j < cities.size(); j++) 
        candidate_cities.push_back(j);
    for (int j = 1; j < cities.size(); j++) {
        int r = rand() % candidate_cities.size();
        temp_path.push_back(candidate_cities[r]);
        candidate_cities.erase(candidate_cities.begin() + r);
    }
    temp_path.push_back(temp_path[0]);
    population.push_back(temp_path);
}
```

- mutation根據路徑隨機交換兩個城市
```cpp
// We don't want to mutate the first and last position
int i = rand() % (target.size() - 2) + 1;
int j = rand() % (target.size() - 2) + 1;
while (i == j)
    j = rand() % (target.size() - 2) + 1;
int temp = target[i];
target[i] = target[j];
target[j] = temp;
return target;
```

- Order crossover
```cpp
vector<vector<int>> GA::orderCrossover(vector<int> father, vector<int> mother) {
    // Select a range of fixed path
    int point1 = rand() % father.size();
    int point2 = rand() % father.size();
    while (point1 == point2)
        point2 = rand() & father.size();
    if (point1 > point2) {
        int t = point2;
        point2 = point1;
        point1 = t;
    }
    // Loop through parents and swap not fixed cities
    vector<int> child1, child2;
    for (int i = 0; i < father.size(); i++) {
        // Keep the start city
        if (i == 0 || i == father.size() - 1) {
            child1.push_back(father[0]);
            child2.push_back(mother[0]);
        }
        else if (i < point1 || i > point2) {
            child1.push_back(-1);
            child2.push_back(-1);
        }
        else {
            child1.push_back(father[i]);
            child2.push_back(mother[i]);
        }
    }
    for (int i = 0; i < child1.size(); i++) {
        if (child1[i] == -1) {
            for (int j = 0; j < mother.size(); j++) {
                if (!count(child1.begin(), child1.end(), mother[j])) {
                    child1[i] = mother[j];
                    break;
                }
            }
        }
    }
    for (int i = 0; i < child2.size(); i++) {
        if (child2[i] == -1) {
            for (int j = 0; j < father.size(); j++) {
                if (!count(child2.begin(), child2.end(), father[j])) {
                    child2[i] = father[j];
                    break;
                }
            }
        }
    }
    return {child1, child2};
}
```

- Cycle crossover
```cpp
vector<vector<int>> GA::cycleCrossover(vector<int> father, vector<int> mother) {
    vector<bool> path_in_cycle(father.size());
    // Loop through path to build cycle
    int start_city = father[0];
    int current_city = mother[0];
    path_in_cycle[0] = true;
    while (current_city != start_city) {
        for (int i = 0; i < father.size(); i++) {
            if (father[i] == current_city) {
                path_in_cycle[i] = true;
                current_city = mother[i];
                break;
            }
        }
    }
    // Swap cities not in cycle
    for (int i = 0; i < father.size(); i++) {
        if (!path_in_cycle[i]) {
            int temp = father[i];
            father[i] = mother[i];
            mother[i] = temp;
        }
    }
    return {father, mother};
}
```

- Partially mapped crossover
```cpp
vector<vector<int>> GA::partiallyMappedCrossover(vector<int> father, vector<int> mother) {
    // Remove last city for crossover, the city will be added back after crossover
    father.pop_back();
    mother.pop_back();
    // 2 point crossover
    int point1 = rand() % father.size();
    int point2 = rand() & father.size();
    while (point1 == point2)
        point2 = rand() & father.size();
    if (point1 > point2) {
        int t = point2;
        point2 = point1;
        point1 = t;
    }
    // Swap the selected area, build list
    vector<vector<int>> list;
    for (int i = point1; i <= point2; i++) {
        int temp = father[i];
        father[i] = mother[i];
        mother[i] = temp;
        list.push_back({father[i], mother[i]});
    }
    // Merge list
    vector<int> remove_list;
    for (int i = 0; i < list.size(); i++) {
        bool list_updated = false;
        for (int j = i + 1; j < list.size(); j++) {
            if (count(remove_list.begin(), remove_list.end(), j)) {
                continue;
            }
            for (int k = 0; k < list[j].size(); k++) {
                if (count(list[i].begin(), list[i].end(), list[j][k])) {
                    for (int l = 0; l < list[j].size(); l++) {
                        if (!count(list[i].begin(), list[i].end(), list[j][l])) {
                            list[i].push_back(list[j][l]);
                            list_updated = true;
                        }
                        if (!count(remove_list.begin(), remove_list.end(), j))
                            remove_list.push_back(j);
                    }
                }
            }
            // If list is updated, we need to revisit all lists
            if (list_updated) {
                j = i;
                list_updated = false;
            }
        }
    }
    sort(remove_list.begin(), remove_list.end());
    for (int j = remove_list.size() - 1; j >= 0; j--) {
        list.erase(list.begin() + remove_list[j]);
    }
    // Fix the rest area
    for (int i = 0; i < father.size(); i++) {
        if (i >= point1 && i <= point2)
            continue;
        // Check if there is a same city in selected area, +1 in second parameter will check point1 ~ point2
        if (count(father.begin() + point1, father.begin() + point2 + 1, father[i])) {
            for (int j = 0; j < list.size(); j++) {
                if (count(list[j].begin(), list[j].end(), father[i])) {
                    for (int k = 0; k < list[j].size(); k++) {
                        if (!count(father.begin(), father.end(), list[j][k])) {
                            father[i] = list[j][k];
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    for (int i = 0; i < mother.size(); i++) {
        if (i >= point1 && i <= point2)
            continue;
        // Check if there is a same city in selected area, +1 in second parameter will check point1 ~ point2
        if (count(mother.begin() + point1, mother.begin() + point2 + 1, mother[i])) {
            for (int j = 0; j < list.size(); j++) {
                if (count(list[j].begin(), list[j].end(), mother[i])) {
                    for (int k = 0; k < list[j].size(); k++) {
                        if (!count(mother.begin(), mother.end(), list[j][k])) {
                            mother[i] = list[j][k];
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    // Add the last city back according to crossover result
    father.push_back(father.front());
    mother.push_back(mother.front());

    if (!isValidPath(father)) {
        cout << "father not valid" << endl;
        printPath(father);
    }
    if (!isValidPath(mother)) {
        cout << "mother not valid" << endl;
        printPath(mother);
    }
    return {father, mother};
}
```
---
