#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <climits>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/**
 * K-means Algorithm
 */
class KmeansPR {
    protected:
        int k;
        int dimension;
        double bestScore;
        const double REMOVE_BOUND = 0.8;
        vector<int> cluster_id;
        vector<bool> fixed;
        vector<vector<double>> centroids;
        vector<vector<double>> iris;
        vector<vector<double>> best;
        vector<double> result;
        double distance(vector<double> point, vector<double> centroid);
        int nearestCluster(vector<double> point, vector<vector<double>> centroids);
        double evaluate(vector<int> cluster_id, vector<vector<double>> cluster_avg);
    public:
        KmeansPR(time_t randseed, int k, char const *seedfile);
        double getBestScore();
        vector<double> run(int iterations);
};
