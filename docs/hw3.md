---
tags: oslab, HW
---

# 作業三

---

### Tabu Search 解 Onemax Problem

#### 執行過程
```bash
$ ./main ts 30 1000 100 "" 10 # tabu list size: 10
```

#### 找到最佳解
- 找到最佳解：100
- 30run最佳解之平均：100

#### 程式說明

- 在class中新增一些需要用到的變數跟方法:
```cpp
private:
    int tabu_list_size;
    deque<vector<bool>> tabu_list; // 用STL的deque作為tabu list
    vector<bool> transitionOnlyNeighbor();
    vector<bool> getNotTabuLeft(vector<bool> arr);
    vector<bool> getNotTabuRight(vector<bool> arr);
    vector<bool> getNotTabuRandom(vector<bool> arr);
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
```cpp
vector<bool> TS::getNotTabuRandom(vector<bool> arr) {
    do {
        int i = rand() % arr.size();
        arr[i] = !arr[i];
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



