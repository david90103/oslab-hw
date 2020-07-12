---
tags: oslab, HW
---

# 作業二

---

### Simlated Annealing 解 Onemax Problem

#### 執行過程
```bash
$ cd one-max
$ make
$ ./main sa 30 300 100 "" 1.0 0.0001 # max temp: 1.0, min temp: 0.0001
```

#### 找到最佳解
- 找到最佳解：62
- 30run最佳解之平均：51.6333

#### 程式說明

- 在每次呼叫transition()取得下個enum後，檢查objective value有沒有比較好，有的話接受這個解，沒有的話透過sa的方式有機率的接受這個解
```cpp
/**
 * Acceptance of SA
 * https://en.wikipedia.org/wiki/Simulated_annealing
 */
inline double SA::acceptance(int current_eval, int new_eval) {
    return exp(-((double) current_eval - new_eval) / temperature);
}

```
```cpp
// 每個iteration呼叫transition()後
if (new_enum_eval >= score) {
    arr = new_enum;
    score = new_enum_eval;
} else {
    if ((double) rand() / RAND_MAX < acceptance(score, new_enum_eval)) {
        arr = new_enum;
        score = new_enum_eval;
    }
}
temperature *= 0.99;
```

---

### Tabu Search 解 Onemax Problem

#### 執行過程
```bash
$ ./main ts 30 3000 100 "" 10 # tabu list size: 10
```

#### 找到最佳解
- 找到最佳解：60
- 30run最佳解之平均：53.6

#### 程式說明

- 在class中新增一些需要用到的變數跟方法:
```cpp
private:
    int tabu_list_size;
    deque<vector<bool>> tabu_list; // 用STL的deque作為tabu list
    vector<bool> getNotTabuLeft(vector<bool> arr);
    vector<bool> getNotTabuRight(vector<bool> arr);
    inline bool inTabuList(vector<bool> target);
```

- 將原本hc使用的transition()更新成tabu search可以使用
```cpp
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
```
```cpp
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
```

- 在每個iteration做完transition()及evaluation()後，更新tabu list
```cpp
// Update tabu list
tabu_list.push_back(arr);
if (tabu_list.size() > tabu_list_size) {
    tabu_list.pop_front();
}
```



