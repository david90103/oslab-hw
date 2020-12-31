#include "lib1dc.hpp"


void TSPAlgorithm1DC::initCircle(vector<vector<double>> cities) {
    double min = DBL_MAX;
    double max = -1;
    centerX = centerY = 0;
    radius = 0;
    for (int i = 0; i < cities.size(); i++) {
        centerX += cities[i][0];
        centerY += cities[i][1];
        if (cities[i][0] < min)
            min = cities[i][0];
        if (cities[i][1] < min)
            min = cities[i][1];
        if (cities[i][0] > max)
            max = cities[i][0];
        if (cities[i][1] > max)
            max = cities[i][1];
    }
    centerX /= cities.size();
    centerY /= cities.size();
    radius = (max - min) / 2;
}

vector<int> TSPAlgorithm1DC::convertToPath(vector<double> angles) {
    return convertToPath(angles, cities, radius);
}

vector<int> TSPAlgorithm1DC::convertToPathNormalized(vector<double> angles) {
    return convertToPath(angles, cities_normalized, radius_normalized);
}

vector<int> TSPAlgorithm1DC::convertToPath(vector<double> angles, vector<vector<double>> cities, double radius) {
    vector<int> path;
    vector<int> candidate_cities;
    for (int i = 0; i < cities.size(); i++)
        candidate_cities.push_back(i);
    // Convert angles to coordinates on the circle
    vector<vector<double>> coordinates;
    for (int i = 0; i < angles.size(); i++) {
        coordinates.push_back({
            centerX + radius * cos(angles[i] * 2 * PI),
            centerY + radius * sin(angles[i] * 2 * PI)
        });
    }
    // Random select start index
    // int offset = rand() % cities.size();
    for (int i = 0; i < angles.size(); i++) {
        vector<double> position = {coordinates[i][0], coordinates[i][1]};
        int next_city = findNearest(position, candidate_cities, cities);
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
