#ifndef LIB1DC_H_
#define LIB1DC_H_

#include "lib.hpp"

#define PI 3.14159265

/**
 * TSPAlgorithm1DC class
 * This is an abstract for tsp problem using a circle mapping encoding method,
 * the degree value of every sample points are encoded in the algorithm.
 */
class TSPAlgorithm1DC : public TSPAlgorithm {
protected:
    double centerX;
    double centerY;
    double radius;
    double radius_normalized = 0.5;
    void initCircle(vector<vector<double>> cities);
    vector<int> convertToPath(vector<double> angles);
    vector<int> convertToPathNormalized(vector<double> angles);
    vector<int> convertToPath(vector<double> angles, vector<vector<double>> cities, double radius);
public:
    virtual vector<double> run(int generations) = 0;
};

#endif // LIB1DC_H_
