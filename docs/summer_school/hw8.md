---
tags: oslab, HW
---

# 作業八

---

### 實作Kmeans with Pattern Reduction在Iris datase

#### 執行過程
```bash
$ cd iris
$ make
$ ./main kmeans 30 100 3 "iris.data"
$ ./main kmeans-pr 30 100 3 "iris.data"

```

#### 找到最佳解 (30 run, each run 100 iterations)

- 找到最佳解：78.9408 (kmeans), 79.8478 (kmeans with PR)
- 30run最佳解之平均：91.7 (kmeans), 96.6 (kmeans with PR)

#### 執行時間比較
- Kmeans: 1.053 seconds
- Kmeans with PR: 0.345 seconds
- 加入PR後減少運行時間67.4%


#### 程式說明

- 為了達到移除冗餘計算，需要在kmeans算法中加入一些額外資訊
```cpp
// 移除上限
const double REMOVE_BOUND = 0.8;
// 移除連續 3 iterations 分到相同 cluster 的 data
const int REMOVE_AFTER_ITER = 3;
// 計算已被移除的數量
int removed_count;
// 紀錄各data被分到相同 cluster 的次數
vector<int> same_cluster_count;
// 紀錄data是否被移除
vector<bool> removed;
```

- 每個iteration計算mean point前，依照```removed```判斷是否要將data重新分類

```cpp
prev_cluster_id = cluster_id;
cluster_id.clear();
for (int i = 0; i < iris.size(); i++) {
    if (removed[i]) {
        cluster_id.push_back(prev_cluster_id[i]);
    } else {
        cluster_id.push_back(nearestCluster(iris[i], centroids));
        same_cluster_count[i]++;
        if ((double) removed_count / iris.size() < REMOVE_BOUND && 
            same_cluster_count[i] >= REMOVE_AFTER_ITER) {
            removed[i] = true;
            removed_count++;
        }
    }
}
```

---