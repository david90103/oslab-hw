---
tags: oslab, HW
---

# 作業一

---

### ES,一個小時內one-max problem 觀察可以跑到幾個bit?

#### 執行結果

```
Iteration: 17680000000 Best score: 34
Iteration: 17690000000 Best score: 34
Iteration: 17700000000 Best score: 34
Iteration: 17710000000 Best score: 34
Iteration: 17720000000 Best score: 34
Iteration: 17730000000 Best score: 34
Iteration: 17740000000 Best score: 34
Iteration: 17750000000 Best score: 34
Iteration: 17760000000 Best score: 34
Iteration: 17770000000 Best score: 34
```

---

### HC實作100個bit的one-max problem

#### 執行過程
```bash
$ cd one-max
$ make
$ ./search.sh hc 30 100 100 ""
$ python merge_output.py hc 30 100 100
$ gnuplot -p plot_hc.gp
```

#### 程式說明

- 初始化：透過seed初始化，如果沒有找到seed，則隨機產生

```cpp
HC::HC(int bits, char *seedfile) {
    char c;
    FILE *fp = fopen(seedfile, "r");
    srand(time(NULL));
    bestScore = 0;
    size = bits;
    arr = (bool *) malloc(sizeof(bool) * size);
    best = (bool *) malloc(sizeof(bool) * size);
    for (int i = 0; i < size; i++) {
        if (fp) {
            fscanf(fp, "%c", &c);
            arr[i] = best[i] = (c == '0') ? false : true;
        } else {
            arr[i] = best[i] = rand() & 1;
        }
    }
    if (fp) {
        fclose(fp);
    }
}
```

- 透過+1或-1取得下一個enum，-1時需要檢查是否為0

```cpp
if (rand() % 2 == 0) {
    // add 1
    carry = new_arr[i] & 1;
    new_arr[i] = !new_arr[i];
    while (carry && i > 0) {
        i--;
        temp = new_arr[i];
        new_arr[i] = carry ^ temp;
        carry = carry & temp;
    }
} else {
    // minus 1
    if (!isZero()) {
        if (!new_arr[i]) {
            do {
                i--;
                temp = new_arr[i];
                new_arr[i] = true;
            } while (!temp);
            new_arr[i] = false;
        }
        new_arr[size - 1] = !new_arr[size - 1];
    }
}
```

- 每個run執行完要sleep一下避免用到同一個rand()的結果

```bash
# search.sh
$EXE $ITERATIONS $BITS $SEEDFILE
sleep 1
```

- 跑完30run之後利用python腳本整理資料，再用gnuplot畫出收斂圖

```bash
$ python merge_output.py hc 30 100 100
$ gnuplot -p plot_hc.gp
```

---
