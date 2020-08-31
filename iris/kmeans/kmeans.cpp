#include "kmeans.hpp"

Kmeans::Kmeans(time_t randseed, int k, char const *seedfile) {
    string line;
    srand(randseed);
    bestScore = INT_MAX;
    this->k = k;
    ifstream fp(seedfile);
    while (getline(fp, line)) {
        if (isdigit(line.front())) {
            vector<double> split;
            string pattern = ",";
            line += pattern;
            for (int i = 0; i < line.size(); i++) {
                int pos = (int) line.find(pattern, i);
                if (pos != string::npos && isdigit(line[i])) {
                    split.push_back(atof(line.substr(i, pos - i).c_str()));
                    i = pos + (int) pattern.size() - 1;
                }
            }
            iris.push_back(split);
        }
    }
    dimension = iris[0].size();
    // iris_normalized = normalizeDataset(iris);
}

// vector<vector<double>> Kmeans::normalizeDataset(vector<vector<double>> dataset) {
//     double max, min;
//     for (int j = 0; j < dimension; j++) {
//         max = -1;
//         min = DBL_MAX;
//         for (int i = 0; i < dataset.size(); i++) {
//             if (dataset[i][j] < min) {
//                 min = dataset[i][j];
//             }
//             if (dataset[i][j] > max) {
//                 max = dataset[i][j];
//             }
//         }
//         for (int i = 0; i < dataset.size(); i++) {
//             dataset[i][j] = (dataset[i][j] - min) / (max - min);
//         }
//     }
//     return dataset;
// }

double Kmeans::getBestScore() {
    return bestScore;
}

inline double Kmeans::distance(vector<double> point, vector<double> centroid) {
    double sum = 0;
    for (int i = 0; i < dimension; i++) {
        sum += pow(point[i] - centroid[i], 2);
    }
    return sqrt(sum);
}

int Kmeans::nearestCluster(vector<double> point, vector<vector<double>> centroids) {
    int nearest_cluster;
    double min_distance = DBL_MAX, temp;
    for (int i = 0; i < k; i++) {
        temp = distance(point, centroids[i]);
        if (temp < min_distance) {
            min_distance = temp;
            nearest_cluster = i;
        }
    }
    return nearest_cluster;
}

double Kmeans::evaluate(vector<int> cluster_id, vector<vector<double>> cluster_avg) {
    double sum = 0;
    for (int i = 0; i < iris.size(); i++) {
        for (int j = 0; j < dimension; j++) {
            sum += pow(iris[i][j] - cluster_avg[cluster_id[i]][j], 2);
        }
    }
    return sum;
}

// double Kmeans::evaluateNormalized(vector<int> cluster_id, vector<vector<double>> cluster_avg) {
//     double sum = 0;
//     for (int i = 0; i < iris_normalized.size(); i++) {
//         for (int j = 0; j < dimension; j++) {
//             sum += pow(iris_normalized[i][j] - cluster_avg[cluster_id[i]][j], 2);
//         }
//     }
//     return sum;
// }

// vector<vector<double>> Kmeans::unormalizeCentroids(vector<vector<double>> cluster_avg) {
//     double max, min;
//     for (int j = 0; j < dimension; j++) {
//         max = -1;
//         min = DBL_MAX;
//         for (int i = 0; i < iris.size(); i++) {
//             if (iris[i][j] < min) {
//                 min = iris[i][j];
//             }
//             if (iris[i][j] > max) {
//                 max = iris[i][j];
//             }
//         }
//         for (int i = 0; i < cluster_avg.size(); i++) {
//             cluster_avg[i][j] = cluster_avg[i][j] * (max - min) + min;
//         }
//     }
//     return cluster_avg;
// }

vector<double> Kmeans::run(int iterations) {
    double temp;
    vector<double> dim_zeros(dimension, 0);
    vector<int> cluster_count;
    vector<vector<double>> cluster_avg;
    // Random pick k points as centroids
    for (int i = 0; i < k; i++) {
        centroids.push_back(iris[rand() % iris.size()]);
    }
    // Repeat clustering points and calculating new centroids
    for (int iter = 0; iter < iterations; iter++) {
        // Cluster points to nearest centroid
        cluster_id.clear();
        for (int i = 0; i < iris.size(); i++) {
            cluster_id.push_back(nearestCluster(iris[i], centroids));
        }
        // Calculate mean point
        cluster_count = vector<int>(k, 0);
        cluster_avg = vector<vector<double>>(k, dim_zeros);
        for (int i = 0; i < iris.size(); i++) {
            cluster_count[cluster_id[i]]++;
            for (int j = 0; j < dimension; j++) {
                cluster_avg[cluster_id[i]][j] += iris[i][j];
            }
        }
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < dimension; j++) {
                cluster_avg[i][j] /= cluster_count[i];
            }
        }
        // Evaluation
        temp = evaluate(cluster_id, cluster_avg);
        if (temp < bestScore) {
            bestScore = temp;
            best = cluster_avg;
        }
        // Set mean points as new centroids
        centroids = cluster_avg;
        // Save result
        result.push_back(bestScore);
    }
    // if (bestScore > 100) {
    // //     // Print cluster ids
    // //     for (int i = 0; i < iris_normalized.size(); i++) {
    // //         cout << nearestCluster(iris_normalized[i], centroids) << " ";
    // //     }
    // //     cout << endl;
    // //     // Print centroids
    //     best = unormalizeCentroids(best);
    //     for (int i = 0; i < best.size(); i++) {
    //         for (int j = 0; j < best[i].size(); j++) {
    //             cout << best[i][j] << " ";
    //         }
    //         cout << endl;
    //     }
    //     for (int i = 0; i < result.size(); i++) {
    //         cout << result[i] << " ";
    //     }
    // }
    cout << bestScore << endl;
    cout << "Done." << endl;
    return result;
}
