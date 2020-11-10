#include "lib.hpp"


double TSPAlgorithm::evaluateWithDistance(vector<double> coordinates, vector<int> path) {
    double sum_of_distance = 0.0;
    for (int i = 0; i < cities.size(); i++)
        sum_of_distance += distance(cities[path[i]], {coordinates[i * 2], coordinates[i * 2 + 1]});
    return evaluate(path) + sum_of_distance / 100;
}

double TSPAlgorithm::evaluate(vector<int> path) {
    double sum = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        sum += distances[path[i]][path[i + 1]];
    }
    return sum;
}

vector<vector<double>> TSPAlgorithm::readCitiesFromFile(char const *seedfile) {
    ifstream fp(seedfile);
    vector<vector<double>> cities;
    string line;
    while (getline(fp, line)) {
        if (isdigit(line.front())) {
            vector<double> split;
            string pattern = " ";
            line += pattern;
            for (int i = 0; i < line.size(); i++) {
                int pos = (int) line.find(pattern, i);
                if (pos != string::npos) {
                    split.push_back(atof(line.substr(i, pos - i).c_str()));
                    i = pos + (int) pattern.size() - 1;
                }
            }
            split.erase(split.begin());
            cities.push_back(split);
        }
    }
    return cities;
}

vector<int> TSPAlgorithm::convertToPath(vector<double> coordinates) {
    vector<int> path;
    vector<int> candidate_cities;
    for (int i = 0; i < cities.size(); i++) 
        candidate_cities.push_back(i);
    // Random select start index
    // int offset = rand() % cities.size();
    for (int i = 0; i < cities.size(); i++) {
        // vector<double> position = {coordinates[(offset + 2 * i) % cities.size()], coordinates[(offset + 2 * i + 1) % cities.size()]};
        vector<double> position = {coordinates[2 * i], coordinates[2 * i + 1]};
        int next_city = findNearest(position, candidate_cities);
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

double TSPAlgorithm::distance(vector<double> a, vector<double> b) {
    return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
}

int TSPAlgorithm::findNearest(vector<double> position, vector<int> candidate_cities) {
    vector<double> distance_to_position;
    double min_distance = DBL_MAX;
    int min_city = -1;
    for (int i = 0; i < candidate_cities.size(); i++) {
        double dist = distance(position, cities[candidate_cities[i]]);
        if (dist < min_distance) {
            min_distance = dist;
            min_city = candidate_cities[i];
        }
    }
    return min_city;
}
