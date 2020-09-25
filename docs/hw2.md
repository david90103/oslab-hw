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
$ ./main sa 30 500 100 "" 1.0 0.0001 # max temp: 1.0, min temp: 0.0001
```

#### 找到最佳解(只能往左右搜尋, 300 iterations)
- 找到最佳解：62
- 30run最佳解之平均：51.6333


#### 找到最佳解(隨機反轉Bit搜尋, 500 iterations)
- 找到最佳解：100
- 30run最佳解之平均：99.633

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



