---
tags: oslab, HW
---

# 作業七

---

### 1.實作GA在Iris dataset (cluster id encoding, centroid encoding)

#### 執行過程
```bash
$ cd iris
$ make
$ ./main ga 30 100 3 "iris.data" 1 30 0.6 0.01
```

#### 找到最佳解 (30 run, each run 300 iterations)

- 找到最佳解：79.8478
- 30run最佳解之平均：99.3955

#### 程式說明

- 因為兩種encoding方式的fitness, crossover, mutation等操作方式不同，分為兩個class實作
  - ClusterIdGA及CentroidsGA都繼承GA
  - fitness, crossover, mutation都下放到個別class去實作
  - Chromosome struct裡面包含兩種encode方式的參數，兩個class分別使用需要的部分
```cpp
struct Chromosome {
    vector<int> cluster_id_encoded;
    vector<vector<double>> centroids_encoded;
};
```
```cpp
/**
 * Abstract class Genetic Algorithm
 */
class GA {
    protected:
        int clusters;
        int encode_type;
        int population_size;
        int dimension;
        double bestScore;
        double crossover_rate;
        double mutation_rate;
        vector<Chromosome> population;
        vector<vector<double>> iris;
        ...略
};

/**
 * Encode with cluster id
 */
class ClusterIdGA : public GA {
    protected:
        void initPopulation();
        double fitness(Chromosome arr);
        vector<Chromosome> crossover(Chromosome father, Chromosome mother);
        Chromosome mutation(Chromosome);
        ...略
};

/**
 * Encode with centroids position
 */
class CentroidsGA : public GA {
    protected:
        void initPopulation();
        inline double distance(vector<double> point, vector<double> centroid);
        Chromosome convertToClusterId(Chromosome);
        double fitness(Chromosome arr);
        vector<Chromosome> crossover(Chromosome father, Chromosome mother);
        Chromosome mutation(Chromosome);
        ...略
};
```

- Main loop, Roulette Wheel selection, Tournament selection皆沿用作業三的code，只修改回傳的參數
```cpp
Chromosome rouletteWheel();
Chromosome tournament();
```

- Cluster id encoding的fitness計算
```cpp
double GA::fitness_cluster_id(vector<int> arr) {
    double result = 0;
    vector<double> temp(dimension, 0);
    vector<vector<double>> cluster_avg(clusters, temp);
    vector<int> cluster_count(clusters, 0);
    for (int i = 0; i < arr.size(); i++) {
        cluster_count[arr[i]]++;
        for (int j = 0; j < dimension; j++) {
            cluster_avg[arr[i]][j] += iris[i][j];
        }
    }
    for (int i = 0; i < clusters; i++) {
        for (int j = 0; j < dimension; j++) {
            cluster_avg[i][j] /= cluster_count[i];
        }
    }
    for (int i = 0; i < arr.size(); i++) {
        for (int j = 0; j < dimension; j++) {
            result += pow(iris[i][j] - cluster_avg[arr[i]][j], 2);
        }
    }
    return result;
}

double ClusterIdGA::fitness(Chromosome arr) {
    return fitness_cluster_id(arr.cluster_id_encoded);
}
```

- Centroids encoding的fitness計算
  - 因為已經有cluster id計算fitness的方式，將centroids encoding轉為cluster id encoding再呼叫原本的fitness function即可
```cpp
/**
 * Fitness computation process includes 2 phases
 * 1. Generate cluster id encoded chromosome by clustering each points to nearest centroids
 * 2. Call the fitness function with cluster_id_encoded chromosome as parameter
 */
double CentroidsGA::fitness(Chromosome arr) {
    return fitness_cluster_id(convertToClusterId(arr).cluster_id_encoded);
}

Chromosome CentroidsGA::convertToClusterId(Chromosome arr) {
    int nearest_cluster;
    double min_distance, temp;
    for (int i = 0; i < iris.size(); i++) {
        min_distance = DBL_MAX;
        for (int j = 0; j < clusters; j++) {
            temp = distance(iris[i], arr.centroids_encoded[j]);
            if (temp < min_distance) {
                min_distance = temp;
                nearest_cluster = j;
            }
        }
        arr.cluster_id_encoded.push_back(nearest_cluster);
    }
    return arr;
}
```

- Cluster id encoding的crossover為找一pivot去組合兩個parent，mutation是隨機將點分到其他cluster，但這樣跑出來的結果很差
```cpp
vector<Chromosome> ClusterIdGA::crossover(Chromosome father, Chromosome mother) {
    int pivot = rand() % father.cluster_id_encoded.size();
    Chromosome child_a;
    Chromosome child_b;
    child_a.cluster_id_encoded = vector<int>(father.cluster_id_encoded.begin(), father.cluster_id_encoded.begin() + pivot);
    child_b.cluster_id_encoded = vector<int>(mother.cluster_id_encoded.begin(), mother.cluster_id_encoded.begin() + pivot);
    child_a.cluster_id_encoded.insert(child_a.cluster_id_encoded.end(), mother.cluster_id_encoded.begin() + pivot, mother.cluster_id_encoded.end());
    child_b.cluster_id_encoded.insert(child_b.cluster_id_encoded.end(), father.cluster_id_encoded.begin() + pivot, father.cluster_id_encoded.end());
    return vector<Chromosome> {child_a, child_b};
}

inline Chromosome ClusterIdGA::mutation(Chromosome target) {
    int i = rand() % target.cluster_id_encoded.size();
    int new_cluster = rand() % clusters;
    while (new_cluster == target.cluster_id_encoded[i]) {
        new_cluster = rand() % clusters;
    }
    target.cluster_id_encoded[i] = new_cluster;
    return target;
}
```

- Centroids encoding的crossover會隨機組合centroids的每個dimension，並且參照下面論文的方式做mutation
  - [COMPARATIVE ANALYSIS OF K-MEANS AND GENETIC
ALGORITHM BASED DATA CLUSTERING](https://bit.ly/2FshxiP) 
```cpp
vector<Chromosome> CentroidsGA::crossover(Chromosome father, Chromosome mother) {
    Chromosome child_a, child_b;
    vector<vector<double>> mask;
    vector<double> temp_a, temp_b;
    for (int i = 0; i < clusters; i++) {
        temp_a.clear();
        temp_b.clear();
        for (int j = 0; j < dimension; j++) {
            if (rand() & 1) {
                temp_a.push_back(father.centroids_encoded[i][j]);
                temp_b.push_back(mother.centroids_encoded[i][j]);
            } else {
                temp_a.push_back(mother.centroids_encoded[i][j]);
                temp_b.push_back(father.centroids_encoded[i][j]);
            }
        }
        child_a.centroids_encoded.push_back(temp_a);
        child_b.centroids_encoded.push_back(temp_b);
    }
    return vector<Chromosome> {child_a, child_b};
}

/**
 * Mutation of centroids encoded chromosome
 * for each dimension as v
 * v = v +/- 2 * delta * v
 * https://bit.ly/2FshxiP
 */
inline Chromosome CentroidsGA::mutation(Chromosome target) {
    int i = rand() % target.centroids_encoded.size();
    double delta;
    for (int j = 0; j < dimension; j++) {
        delta = (double) rand() / RAND_MAX;
        if (rand() & 1) {
            target.centroids_encoded[i][j] = target.centroids_encoded[i][j] + 2 * delta * target.centroids_encoded[i][j];
        } else {
            target.centroids_encoded[i][j] = target.centroids_encoded[i][j] - 2 * delta * target.centroids_encoded[i][j];
        }
    }
    return target;
}
```

---

### 2.實作K-means在Iris dataset

#### 執行過程
```bash
$ cd iris
$ make
$ ./main kmeans 30 20 3 "iris.data"
```

#### 找到最佳解 (30 run, each run 20 iterations)

- 找到最佳解：79.3954
- 30run最佳解之平均：88.1003

#### 程式說明

- 隨機產生三個點作為群心
```cpp
// Random pick k points as centroids
for (int i = 0; i < k; i++) {
    centroids.push_back(iris_normalized[rand() % iris_normalized.size()]);
}
```

- 每個iteration重複分群、計算群心、更新群心
```cpp
for (int iter = 0; iter < iterations; iter++) {
    // Cluster points to nearest centroid
    cluster_id.clear();
    for (int i = 0; i < iris_normalized.size(); i++) {
        cluster_id.push_back(nearestCluster(iris_normalized[i], centroids));
    }
    // Calculate mean point
    cluster_count = vector<int>(k, 0);
    cluster_avg = vector<vector<double>>(k, dim_zeros);
    for (int i = 0; i < iris_normalized.size(); i++) {
        cluster_count[cluster_id[i]]++;
        for (int j = 0; j < dimension; j++) {
            cluster_avg[cluster_id[i]][j] += iris_normalized[i][j];
        }
    }
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < dimension; j++) {
            cluster_avg[i][j] /= cluster_count[i];
        }
    }
    // Evaluation
    temp = evaluate(cluster_id, unormalizeCentroids(cluster_avg));
    if (temp < bestScore) {
        bestScore = temp;
        best = cluster_avg;
    }
    // Set mean points as new centroids
    centroids = cluster_avg;
    // Save result
    result.push_back(bestScore);
}
```

---

#### Questions

- GA收斂結果差
  - crossover方式錯誤?
  - mutation容易超出範圍?

- K-means有時候收斂失敗卡在140+
  - 有時候超過150，其中一個centroid座標是nan，變成k = 2
  - 經常卡在143.04
  - dataset有做normalization反而結果變差?

---
