#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
    ifstream fp("eil51.tsp");
    ofstream fp_c("output_cities.txt");
    ofstream fp_p("output_path.txt");
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

    for (int i = 0; i < cities.size(); i++) {
        fp_c << cities[i][0] << " " << cities[i][1] << endl;
    }

    if (argc != 53) {
        cout << "Usage: ./draw <path>" << endl;
        return -1;
    }

    for (int i = 1; i < argc - 1; i++) {
        fp_p << cities[atoi(argv[i])][0] << " " << cities[atoi(argv[i])][1] << endl;
        fp_p << cities[atoi(argv[i + 1])][0] << " " << cities[atoi(argv[i + 1])][1] << endl << endl;
    }
    cout << "Done." << endl;
    return 0;
}
