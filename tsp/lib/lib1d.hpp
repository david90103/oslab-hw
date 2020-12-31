#ifndef LIB1D_H_
#define LIB1D_H_

#include "lib.hpp"

/**
 * TSPAlgorithm1D class
 * This is an abstract for tsp problem using random key 
 * encoding with encodes the priority value of every cities, 
 * the priority values will then convert into path string.
 */
class TSPAlgorithm1D : public TSPAlgorithm {
protected:
    vector<int> convertToPath(vector<double> coordinates);
    vector<int> convertToPath(vector<double> coordinates, vector<vector<double>> cities);
public:
    virtual vector<double> run(int generations) = 0;
};

#endif // LIB1D_H_
