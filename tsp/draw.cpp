#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <math.h>
#include <float.h>
using namespace std;

vector<int> path;
vector<vector<double>> cities;
vector<vector<double>> distances;

double distance(vector<double> city_a, vector<double> city_b) {
    return sqrt(pow(city_a[0] - city_b[0], 2) + pow(city_a[1] - city_b[1], 2));
}

double evaluate(vector<int> path) {
    double sum = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        sum += distances[path[i]][path[i + 1]];
    }
    return sum;
}

vector<vector<double>> normalize(vector<vector<double>> cities) {
    double max_x = -1;
    double max_y = -1;
    double min_x = DBL_MAX;
    double min_y = DBL_MAX;
    for (int i = 0; i < cities.size(); i++) {
        if (cities[i][0] < min_x) 
            min_x = cities[i][0];
        if (cities[i][1] < min_y) 
            min_y = cities[i][1];
        if (cities[i][0] > max_x) 
            max_x = cities[i][0];
        if (cities[i][1] > max_y) 
            max_y = cities[i][1];
    }
    for (int i = 0; i < cities.size(); i++) {
        cities[i][0] = (cities[i][0] - min_x) / (max_x - min_x);
        cities[i][1] = (cities[i][1] - min_y) / (max_y - min_y);
    }
    return cities;
}

int main(int argc, char *argv[]) {
    ifstream fp("data/eil51.tsp");
    ofstream fp_c("output_cities.txt");
    ofstream fp_p("output_path.txt");
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

    // Normalize cities
    // cities = normalize(cities);

    // Initialize distance table
    for (int i = 0; i < cities.size(); i++) {
        distances.push_back(vector<double>());
        for (int j = 0; j < cities.size(); j++) {
            distances[i].push_back(distance(cities[i], cities[j]));
        }
    }

    for (int i = 0; i < cities.size(); i++) {
        fp_c << cities[i][0] << " " << cities[i][1] << endl;
    }

    if (argc != cities.size() + 2) {
        cout << "Usage: ./draw <path>" << endl;
        return -1;
    }

    for (int i = 1; i < argc - 1; i++) {
        path.push_back(atoi(argv[i]));
        fp_p << cities[atoi(argv[i])][0] << " " << cities[atoi(argv[i])][1] << endl;
        fp_p << cities[atoi(argv[i + 1])][0] << " " << cities[atoi(argv[i + 1])][1] << endl << endl;
    }
    path.push_back(atoi(argv[argc - 1]));
    cout << "Distance: " << evaluate(path) << endl;
    cout << "Done." << endl;
    return 0;
}
