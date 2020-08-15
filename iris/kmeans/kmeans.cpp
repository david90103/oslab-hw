#include "kmeans.hpp"

Kmeans::Kmeans(time_t randseed, int k, char const *seedfile) {
    string line;
    // double max, min;
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
    /**
     * TODO: Normalize each dimension
     */
    // iris_normalized = iris;
    // for (int j = 0; j < dimension; j++) {
    //     max = -1;
    //     min = DBL_MAX;
    //     for (int i = 0; i < iris_normalized.size(); i++) {
    //         if (iris_normalized[i][j] < min) {
    //             min = iris_normalized[i][j];
    //         }
    //         if (iris_normalized[i][j] > max) {
    //             max = iris_normalized[i][j];
    //         }
    //     }
    //     for (int i = 0; i < iris_normalized.size(); i++) {
    //         iris_normalized[i][j] = (iris_normalized[i][j] - min) / (max - min);
    //     }
    // }
}

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
        bestScore = evaluate(cluster_id, cluster_avg);
        best = cluster_avg;
        // Set mean points as new centroids
        centroids = cluster_avg;
        // Save result
        result.push_back(bestScore);
    }
    for (int i = 0; i < best.size(); i++) {
        for (int j = 0; j < best[i].size(); j++) {
            cout << best[i][j] << " ";
        }
        cout << endl;
    }
    cout << bestScore << endl;
    cout << "Done." << endl;
    return result;
}
