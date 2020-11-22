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
        // Start from city 0
        temp_path.push_back(0);
        candidate_cities.clear();
        for (int j = 1; j < cities.size(); j++) 
            candidate_cities.push_back(j);
        for (int j = 1; j < cities.size(); j++) {
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
    // Remove last city for crossover, the city will be added back after crossover
    father.pop_back();
    mother.pop_back();
    // 2 point crossover
    int point1 = rand() % father.size();
    int point2 = rand() & father.size();
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
    // Fix the rest area
    for (int i = 0; i < father.size(); i++) {
        if (i >= point1 && i <= point2)
            continue;
        // Check if there is a same city in selected area, +1 in second parameter will check point1 ~ point2
        if (count(father.begin() + point1, father.begin() + point2 + 1, father[i])) {
            for (int j = 0; j < list.size(); j++) {
                if (count(list[j].begin(), list[j].end(), father[i])) {
                    for (int k = 0; k < list[j].size(); k++) {
                        if (!count(father.begin(), father.end(), list[j][k])) {
                            father[i] = list[j][k];
                            break;
                        }
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
    vector<bool> path_in_cycle(father.size());
    // Loop through path to build cycle
    int start_city = father[0];
    int current_city = mother[0];
    path_in_cycle[0] = true;
    while (current_city != start_city) {
        for (int i = 0; i < father.size(); i++) {
            if (father[i] == current_city) {
                path_in_cycle[i] = true;
                current_city = mother[i];
                break;
            }
        }
    }
    // Swap cities not in cycle
    for (int i = 0; i < father.size(); i++) {
        if (!path_in_cycle[i]) {
            int temp = father[i];
            father[i] = mother[i];
            mother[i] = temp;
        }
    }
    return {father, mother};
}

vector<vector<int>> GA::orderCrossover(vector<int> father, vector<int> mother) {
    // Select a range of fixed path
    int point1 = rand() % father.size();
    int point2 = rand() % father.size();
    while (point1 == point2)
        point2 = rand() & father.size();
    if (point1 > point2) {
        int t = point2;
        point2 = point1;
        point1 = t;
    }
    // Loop through parents and swap not fixed cities
    vector<int> child1, child2;
    for (int i = 0; i < father.size(); i++) {
        // Keep the start city
        if (i == 0 || i == father.size() - 1) {
            child1.push_back(father[0]);
            child2.push_back(mother[0]);
        }
        else if (i < point1 || i > point2) {
            child1.push_back(-1);
            child2.push_back(-1);
        }
        else {
            child1.push_back(father[i]);
            child2.push_back(mother[i]);
        }
    }
    for (int i = 0; i < child1.size(); i++) {
        if (child1[i] == -1) {
            for (int j = 0; j < mother.size(); j++) {
                if (!count(child1.begin(), child1.end(), mother[j])) {
                    child1[i] = mother[j];
                    break;
                }
            }
        }
    }
    for (int i = 0; i < child2.size(); i++) {
        if (child2[i] == -1) {
            for (int j = 0; j < father.size(); j++) {
                if (!count(child2.begin(), child2.end(), father[j])) {
                    child2[i] = father[j];
                    break;
                }
            }
        }
    }
    return {child1, child2};
}

/**
 * Mutation
 * Random swap 2 neighbor cities
 */
inline vector<int> GA::mutation(vector<int> target) {
    // We don't want to mutate the first and last position
    int i = rand() % (target.size() - 3) + 1;
    int temp = target[i];
    target[i] = target[i + 1];
    target[i + 1] = temp;
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
            // a = tournament();
            // b = tournament();
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
        if (gen % 100 == 0 || gen < 20) {
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
    cout << "==========================" << endl;
    cout << "Found Best: " << endl;
    for (int i = 0; i < best.size(); i++) {
        cout << best[i] << " ";
    }
    cout << endl;
    cout << "Distance: " << bestScore << endl;
    if (bestScore < 450) {
        cout << " XD";
    }
    cout << "Done." << endl;
    return result;
}