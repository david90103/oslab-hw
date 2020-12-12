#include "lib1d.hpp"


vector<int> TSPAlgorithm1D::convertToPath(vector<double> coordinates) {
    return convertToPath(coordinates, cities);
}

vector<int> TSPAlgorithm1D::convertToPath(vector<double> coordinates, vector<vector<double>> cities) {
    vector<int> path;
    vector<int> candidate_cities;
    for (int i = 0; i < cities.size(); i++) 
        candidate_cities.push_back(i);
    for (int i = 0; i < cities.size(); i++) {
        int next_city = candidate_cities[0];
        for (int j = 0; j < candidate_cities.size(); j++)
            if (coordinates[candidate_cities[j]] > coordinates[next_city])
                next_city = candidate_cities[j];
        path.push_back(next_city);
        // Remove city from list
        for (vector<int>::iterator iter = candidate_cities.begin(); iter != candidate_cities.end(); iter++) {
            if (*iter == next_city) {
                candidate_cities.erase(iter);
                break;
            }
        }
    }
    path.push_back(path[0]);
    return path;
}
