#include "ga.hpp"

GA::GA(int bits, int population_s, double crossover_r, double mutation_r, char const *crossover_method, char const *seedfile) {
    srand(time(NULL));
    bestScore = DBL_MAX;
    population_size = population_s;
    crossover_rate = crossover_r;
    mutation_rate = mutation_r;
    fitness_values_sum = 0;
    fitness_values = vector<double>(population_size, DBL_MAX);
    cities = readCitiesFromFile(seedfile);
    distances = initDistances(cities);
    population = initPopulation(cities);
    if (strcmp(crossover_method, "pmx") == 0)
        crossover = &GA::partiallyMappedCrossover;
    else if (strcmp(crossover_method, "cx") == 0)
        crossover = &GA::cycleCrossover;
    else // ox
        crossover = &GA::orderCrossover;
    evalPopulation(vector<bool>(population_size, true));
}

vector<vector<double>> GA::readCitiesFromFile(char const *seedfile) {
    vector<vector<double>> cities;
    ifstream fp(seedfile);
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

vector<vector<double>> GA::initDistances(vector<vector<double>> cities) {
    vector<vector<double>> distances;
    for (int i = 0; i < cities.size(); i++) {
        distances.push_back(vector<double>());
        for (int j = 0; j < cities.size(); j++) {
            distances[i].push_back(distance(cities[i], cities[j]));
        }
    }
    return distances;
}

vector<vector<int>> GA::initPopulation(vector<vector<double>> cities) {
    vector<vector<int>> population;
    vector<int> candidate_cities;
    vector<int> temp_path;
    for (int i = 0; i < population_size; i++) {
        temp_path.clear();
        candidate_cities.clear();
        for (int j = 0; j < cities.size(); j++) 
            candidate_cities.push_back(j);
        for (int j = 0; j < cities.size(); j++) {
            int r = rand() % candidate_cities.size();
            temp_path.push_back(candidate_cities[r]);
            candidate_cities.erase(candidate_cities.begin() + r);
        }
        temp_path.push_back(temp_path[0]);
        population.push_back(temp_path);
    }
    return population;
}

void GA::printPath(vector<int> path) {
    for (int i = 0; i < path.size(); i++) 
        cout << path[i] << " ";
    cout << endl;
}

double GA::distance(vector<double> a, vector<double> b) {
    return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
}

/**
 * Evaluate every chromosome in population
 * Parameter is_new_member is a intean list to track if a chromosome
 * in population is created by crossover or directly from parents
 */
void GA::evalPopulation(vector<bool> is_new_member) {
    int temp;
    fitness_values_sum = 0;
    for (int i = 0; i < population.size(); i++) {
        temp = fitness(population[i]);
        fitness_values[i] = temp;
        fitness_values_sum += temp;
        if (fitness_values[i] < bestScore) {
            bestScore = fitness_values[i];
            best = population[i];
        }
        if (is_new_member[i]) {
            result.push_back(bestScore);
        }
    }
}

/**
 * Objective function
 */
int GA::fitness(vector<int> path) {
    double sum = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        sum += distances[path[i]][path[i + 1]];
    }
    return sum;
}

/**
 * Selection using Roulette Wheel
 */
vector<int> GA::rouletteWheel() {
    int sum = 0;
    int target = rand() % fitness_values_sum;
    for (int i = 0; i < population_size; i++) {
        sum += fitness_values[i];
        if (sum >= target) {
            return population[i];
        }
    }
    return population[population_size - 1];
}

/**
 * Selection using Tournament Selection
 */
vector<int> GA::tournament() {
    int i = rand() % population_size;
    int j = rand() % population_size;
    if (fitness_values[i] >= fitness_values[j]) {
        return population[i];
    }
    return population[j];
}

bool GA::isValidPath(vector<int> path) {
    // vector<int> check(7, 0);
    vector<int> check(cities.size(), 0);
    if (path.front() != path.back()) {
        cout << "front != back" << endl;
        return false;
    }
    for (int i = 0; i < path.size(); i++) {
        check[path[i]]++;
    }
    for (int i = 0; i < check.size(); i++) {
        if (i == path.front()) {
            if (check[i] == 2)
                continue;
            else {
                cout << "check of front is not 2" << endl;
                return false;
            }
                
        }
        if (check[i] == 0) {
            cout << "check " << i << " is 0" << endl;
        }
        if (check[i] > 1) {
            cout << "check " << i << " is " << check[i] << endl;
            return false;
        }
    }
    return true;
}

vector<vector<int>> GA::partiallyMappedCrossover(vector<int> father, vector<int> mother) {
    //debug
    // father = {0, 1, 3, 6, 4, 2, 5, 0};
    // mother = {0, 4, 5, 3, 6, 1, 2, 0};
    // father = {1, 2, 3, 4, 5, 6, 7, 0, 8, 1};
    // mother = {3, 7, 5, 1, 6, 8, 2, 4, 0, 3};
    // father = {6, 10, 11, 23, 8, 22, 7, 5, 21, 16, 14, 20, 17, 2, 24, 0, 13, 12, 1, 18, 19, 4, 15, 3, 9, 6};
    // mother = {1, 6, 3, 5, 21, 2, 11, 20, 24, 8, 7, 22, 16, 17, 0, 14, 12, 9, 10, 4, 15, 23, 19, 18, 13, 1};
    // cout << "------- before crossover -------" << endl;
    // printPath(father);
    // printPath(mother);

    //debug
    // vector<int> father_before = father;

    // Remove last city for crossover, the city will be added back after crossover
    father.pop_back();
    mother.pop_back();

    // 2 point crossover
    int point1 = rand() % father.size();
    int point2 = rand() & father.size();

    //debug
    // point1 = 1;
    // point2 = 17;
    // point1 = 1;
    // point2 = 4;
    // cout << point1 << endl;
    // cout << point2 << endl;

    while (point1 == point2)
        point2 = rand() & father.size();
    if (point1 > point2) {
        int t = point2;
        point2 = point1;
        point1 = t;
    }
    // Swap the selected area, build list
    vector<vector<int>> list;
    for (int i = point1; i <= point2; i++) {
        int temp = father[i];
        father[i] = mother[i];
        mother[i] = temp;
        list.push_back({father[i], mother[i]});
    }
    // Merge list
    vector<int> remove_list;
    for (int i = 0; i < list.size(); i++) {
        bool list_updated = false;
        for (int j = i + 1; j < list.size(); j++) {
            if (count(remove_list.begin(), remove_list.end(), j)) {
                continue;
            }
            for (int k = 0; k < list[j].size(); k++) {
                if (count(list[i].begin(), list[i].end(), list[j][k])) {
                    for (int l = 0; l < list[j].size(); l++) {
                        if (!count(list[i].begin(), list[i].end(), list[j][l])) {
                            list[i].push_back(list[j][l]);
                            list_updated = true;
                        }
                        if (!count(remove_list.begin(), remove_list.end(), j))
                            remove_list.push_back(j);
                    }
                }
            }
            // If list is updated, we need to revisit all lists
            if (list_updated) {
                j = i;
                list_updated = false;
            }
        }
    }
    sort(remove_list.begin(), remove_list.end());
    for (int j = remove_list.size() - 1; j >= 0; j--) {
        list.erase(list.begin() + remove_list[j]);
    }

    // print list
    // cout << "list: " << endl;
    // for (int i = 0; i < list.size(); i++) {
    //     for (int j = 0; j < list[i].size(); j++)
    //         cout << list[i][j] << " ";
    //     cout << endl;
    // }

    // Fix the rest area
    for (int i = 0; i < father.size(); i++) {
        if (i >= point1 && i <= point2)
            continue;
        // Check if there is a same city in selected area
        if (count(father.begin() + point1, father.begin() + point2 + 1, father[i])) {
            for (int j = 0; j < list.size(); j++) {
                if (count(list[j].begin(), list[j].end(), father[i])) {
                    for (int k = 0; k < list[j].size(); k++) {
                        // if (i  == father.size() - 1) {
                        //     cout << "<<<< " << list[j][k] << endl;
                        // }
                        if (!count(father.begin(), father.end(), list[j][k])) {
                            father[i] = list[j][k];
                            break;
                        }
                        // if (k == list[j].size() - 1) {
                        //     // print list
                        //     cout << "list: " << endl;
                        //     for (int m = 0; m < list.size(); m++) {
                        //         for (int n = 0; n < list[m].size(); n++)
                        //             cout << list[m][n] << " ";
                        //         cout << endl;
                        //     }
                        //     cout << "found but all exists !!!!" << endl;
                        //     cout << "father before:" << endl;
                        //     for (int m = 0; m < father_before.size(); m++) {
                        //         cout << father_before[m] << " ";
                        //     }
                        //     cout << endl;
                        //     cout << "father after:" << endl;
                        //     for (int m = 0; m < father.size(); m++) {
                        //         cout << father[m] << " ";
                        //     }
                        //     cout << endl;
                        // }
                    }
                    break;
                }
            }
        }
    }
    for (int i = 0; i < mother.size(); i++) {
        if (i >= point1 && i <= point2)
            continue;
        // Check if there is a same city in selected area, +1 in second parameter will check point1 ~ point2
        if (count(mother.begin() + point1, mother.begin() + point2 + 1, mother[i])) {
            for (int j = 0; j < list.size(); j++) {
                if (count(list[j].begin(), list[j].end(), mother[i])) {
                    for (int k = 0; k < list[j].size(); k++) {
                        if (!count(mother.begin(), mother.end(), list[j][k])) {
                            mother[i] = list[j][k];
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    // Add the last city back according to crossover result
    father.push_back(father.front());
    mother.push_back(mother.front());

    if (!isValidPath(father)) {
        cout << "father not valid" << endl;
        printPath(father);
    }
    if (!isValidPath(mother)) {
        cout << "mother not valid" << endl;
        printPath(mother);
    }

    return {father, mother};
}

vector<vector<int>> GA::cycleCrossover(vector<int> father, vector<int> mother) {
    cout << "cx" << endl;
    return {father, mother};
}

vector<vector<int>> GA::orderCrossover(vector<int> father, vector<int> mother) {
    cout << "ox" << endl;
    return {father, mother};
}

inline vector<int> GA::mutation(vector<int> target) {
    // We don't want to mutate the first and last position
    int i = rand() % target.size();
    while (i == 0 || i == target.size() - 1)
        i = rand() % target.size();
    int j = rand() % target.size();
    while (j == 0 || j == target.size() - 1 || j == i)
        j = rand() % target.size();
    int temp = target[i];
    target[i] = target[j];
    target[j] = temp;
    return target;
}

vector<double> GA::run(int generations) {
    for (int gen = 1; gen <= generations; gen++) {
        vector<int> a, b;
        vector<vector<int>> crossover_result;
        vector<vector<int>> new_population;
        // Save a boolean list to track if a chromosome is new or not, just for logging
        vector<bool> new_member_list(population_size, false);

        for (int i = 0; i < population_size / 2; i++) {
            // Selection
            a = rouletteWheel();
            b = rouletteWheel();
            // Crossover
            if ((double) rand() / RAND_MAX < crossover_rate) {
                crossover_result = (this->*crossover)(a, b);
                new_population.insert(new_population.end(), crossover_result.begin(), crossover_result.end());
                new_member_list[i * 2] = new_member_list[i * 2 + 1] = true;
            } else {
                new_population.push_back(a);
                new_population.push_back(b);
            }
        }
        // Mutation
        for (int i = 0; i < new_population.size(); i++) {
            if ((double) rand() / RAND_MAX < mutation_rate) {
                new_population[i] = mutation(new_population[i]);
                new_member_list[i] = true;
            }
        }
        // Apply population and fitness values
        population = new_population;
        evalPopulation(new_member_list);
        
        // Record and log
        if (gen % 10 == 0) {
            cout << "Generation: " << gen << " Best score: " << bestScore << endl;
        }
    }
    // Fix the result size because crossover may cause different evaluations for each population
    int target_evaluations = generations * population_size * crossover_rate;
    int original_size = result.size();
    if (result.size() > target_evaluations) {
        for (int i = 0; i < original_size - target_evaluations; i++) {
            result.pop_back();
        }
    }
    else if (result.size() < target_evaluations) {
        for (int i = 0; i < target_evaluations - original_size; i++) {
            result.push_back(bestScore);
        }
    }
    cout << "Done." << endl;
    return result;
}