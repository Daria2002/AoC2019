#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <stdio.h>
#include <set>
#include <string.h>
#include <bits/stdc++.h>
#include <functional>
#include <algorithm>
#include <math.h>
#include <utility>
#include <unordered_map>
#include <array>

#define ASCII_ZERO 48
#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};

class Intcode_calculator {

    enum Operations {
        SUM = 1,
        MUL = 2,
        SAVE_INPUT = 3,
        OUTPUT = 4,
        JUMP_IF_TRUE = 5,
        JUMP_IF_FALSE = 6,
        LESS_THAN = 7,
        EQUALS = 8,
        RELATIVE_BASE = 9,
        HALT = 99
    };

    enum Modes {
        POSITION = 0,
        IMMIDIATE  = 1,  
        RELATIVE = 2
    };
   
    public:
        std::map<int, std::function<void()>> functions;
        Intcode_calculator(std::vector<long long int> _elements) : elements(_elements)
        {
            last_index = 0;
            // capture class members by saying 'this' in the capture list
            functions.emplace(Operations::SUM, [&]() {
                num_of_params = 3;
                elements[params[2]] = elements[params[0]] + elements[params[1]];
            });

            functions.emplace(Operations::MUL, [&]() {
                num_of_params = 3;
                elements[params[2]] = elements[params[0]] * elements[params[1]];    
            });

            functions.emplace(Operations::SAVE_INPUT, [&]() {
                num_of_params = 1;
                elements[params[0]] = input;
            });

            functions.emplace(Operations::OUTPUT, [&]() {
                num_of_params = 1;
                output = elements[params[0]];
            });

            functions.emplace(Operations::JUMP_IF_TRUE, [&]() {
                num_of_params = 2;
                if(elements[params[0]] != 0) {
                    // because there is num_of_params+1 in for loop
                    num_of_params = -1;
                    i = elements[params[1]];
                }
            });

            functions.emplace(Operations::JUMP_IF_FALSE, [&]() {
                num_of_params = 2;
                if(elements[params[0]] == 0) {
                    // because there is num_of_params+1 in for loop
                    num_of_params = -1;
                    i = elements[params[1]];
                }
            });

            functions.emplace(Operations::LESS_THAN, [&]() {
                num_of_params = 3;
                if(elements[params[0]] < elements[params[1]]) {
                    elements[params[2]] = 1;
                } else {
                    elements[params[2]] = 0;
                }
            });

            functions.emplace(Operations::EQUALS, [&]() {
                num_of_params = 3;
                if(elements[params[0]] == elements[params[1]]) {
                    elements[params[2]] = 1;
                } else {
                    elements[params[2]] = 0;
                }
            });
       
            functions.emplace(Operations::RELATIVE_BASE, [&]() {
                num_of_params = 1;
                relative_base += elements[params[0]];
            });
        }

        long long int calculate(int _input) {
            input = _input; 
            for(i = last_index; i < elements.size() && elements[i] != Operations::HALT; i += num_of_params+1) {
                element = std::to_string(elements[i]);
                int help = 0;
                for(int k = 0; k < 3; k++) {
                    if(element.size() > 4-k) {
                        indices_mode[2-k] = element.at(help++)-ASCII_ZERO;
                    } else {
                        indices_mode[2-k] = 0;
                    }
                }

                for(int j = 0; j < 3; j++) {
                    // last element and it is 99
                    if(i+j+1 >= elements.size()) {
                        params[j] = 0;
                        continue;
                    }
                   
                    if(indices_mode[j] == 0) {
                        params[j] = elements[i+j+1];
                    } else if(indices_mode[j] == 1) {
                        params[j] = i+j+1;
                    } else if(indices_mode[j] == 2) {
                        params[j] = elements[i+j+1]+relative_base;
                    }
                   
                    // if index greater than elements size
                    if(params[j] > elements.size()) {
                        fill_with_zeros(elements, params[j]);
                    }
                }

                functions[element[element.size()-1]-ASCII_ZERO]();
                if(element[element.size()-1]-ASCII_ZERO == 4) {
                    last_index = i + num_of_params + 1;
                    return output;
                }
            }
            return -1;
        }

    private:
        std::array<long long int, 3> params;
        int last_index;
        std::vector<long long int> elements;
        int num_of_params = 1;
        int relative_base = 0;
        std::array<int, 3> indices_mode = {0};
        std::string element;
        long long int output;
        int i;
        int input;
       
        void fill_with_zeros(std::vector<long long int> &elements, long long int index) {
            int size = elements.size();
            for(int i = size; i <= index; i++) {
                elements.push_back(0);
            }
        }
};

std::vector<std::pair<long long, long long>> walls;
std::vector<std::pair<long long, long long>> path;

long long int search_for_oxygen_system(
    std::pair<long long int, long long int> start_coordinate,
    Intcode_calculator &calc,
    int steps) {
    int result;
    std::pair<long long int, long long int> new_coordinate;
    int path_result = 0;

    // next coordinate - north move
    new_coordinate = std::make_pair(start_coordinate.first+1, start_coordinate.second);
    if(std::find(walls.begin(), walls.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == walls.end()
    && std::find(path.begin(), path.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == path.end()) {
        result = calc.calculate(NORTH);
        // hit a wall
        if(result == 0) {
            walls.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
        } 
        // not wall, not oxygen system
        else if(result == 1) {
            path.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
            path_result = search_for_oxygen_system(new_coordinate, calc, steps);
            if(path_result > 0) {
                steps = path_result + 1;
                return steps;
            }
            calc.calculate(SOUTH);
        } 
        // oxygen system
        else if(result == 2) {
            return 1;
        }
    }
    // go back
    new_coordinate = std::make_pair(new_coordinate.first-1, new_coordinate.second);

    // next coordinate - east move
    new_coordinate = std::make_pair(start_coordinate.first, start_coordinate.second+1);
    if(std::find(walls.begin(), walls.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == walls.end()
    && std::find(path.begin(), path.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == path.end()) {
        result = calc.calculate(EAST);

        if(result == 0) {
            walls.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
        } 
        // not wall, not oxygen system
        else if(result == 1) {
            path.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
            path_result = search_for_oxygen_system(new_coordinate, calc, steps);
            if(path_result > 0) {
                steps = path_result + 1;
                return steps;
            }
            calc.calculate(WEST);
        } 
        // oxygen system
        else if(result == 2) {
            return 1;
        }
    }
    // go back
    new_coordinate = std::make_pair(new_coordinate.first, new_coordinate.second-1);

    // next coordinate - south move
    new_coordinate = std::make_pair(start_coordinate.first-1, start_coordinate.second);
    if(std::find(walls.begin(), walls.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == walls.end()
    && std::find(path.begin(), path.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == path.end()) {
        result = calc.calculate(SOUTH);

        // hit a wall
        if(result == 0) {
            walls.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
        } 
        // not wall, not oxygen system
        else if(result == 1) {
            path.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
            path_result = search_for_oxygen_system(new_coordinate, calc, steps);
            if(path_result > 0) {
                steps = path_result + 1;
                return steps;
            }
            calc.calculate(NORTH);
        } 
        // oxygen system
        else if(result == 2) {
            return 1;
        }
    }
    // go back
    new_coordinate = std::make_pair(new_coordinate.first+1, new_coordinate.second);

    // next coordinate - west move
    new_coordinate = std::make_pair(start_coordinate.first, start_coordinate.second-1);
    if(std::find(walls.begin(), walls.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == walls.end()
    && std::find(path.begin(), path.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == path.end()) {
        result = calc.calculate(WEST);

        if(result == 0) {
            walls.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
        } 
        // not wall, not oxygen system
        else if(result == 1) {
            path.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
            path_result = search_for_oxygen_system(new_coordinate, calc, steps);
            if(path_result > 0) {
                steps = path_result + 1;
                return steps;
            }
            calc.calculate(EAST);
        } 
        // oxygen system
        else if(result == 2) {
            return 1;
        }
    }
    // go back
    new_coordinate = std::make_pair(new_coordinate.first, new_coordinate.second+1);

    return steps;
}

std::vector<std::vector<int>> state;
std::pair<long long int, long long int> oxygen_station;

void search_space(std::pair<long long int, long long int> start_coordinate, Intcode_calculator &calc) {
    int result;
    std::pair<long long int, long long int> new_coordinate;
    int path_result = 0;

    // next coordinate - north move
    new_coordinate = std::make_pair(start_coordinate.first+1, start_coordinate.second);
    if(std::find(walls.begin(), walls.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == walls.end()
    && std::find(path.begin(), path.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == path.end()) {
        result = calc.calculate(NORTH);
        // hit a wall
        if(result == 0) {
            walls.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
        } 
        // not wall, not oxygen system
        else if(result == 1) {
            path.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
            search_space(new_coordinate, calc);
            calc.calculate(SOUTH);
        } 
        // oxygen system
        else if(result == 2) {
            oxygen_station = new_coordinate;
            search_space(new_coordinate, calc);
            calc.calculate(SOUTH);
        }
    }

    // next coordinate - east move
    new_coordinate = std::make_pair(start_coordinate.first, start_coordinate.second+1);
    if(std::find(walls.begin(), walls.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == walls.end()
    && std::find(path.begin(), path.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == path.end()) {
        result = calc.calculate(EAST);

        if(result == 0) {
            walls.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
        } 
        // not wall, not oxygen system
        else if(result == 1) {
            path.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
            search_space(new_coordinate, calc);
            calc.calculate(WEST);
        } 
        // oxygen system
        else if(result == 2) {
            oxygen_station = new_coordinate;
            search_space(new_coordinate, calc);
            calc.calculate(WEST);
        }
    }

    // next coordinate - south move
    new_coordinate = std::make_pair(start_coordinate.first-1, start_coordinate.second);
    if(std::find(walls.begin(), walls.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == walls.end()
    && std::find(path.begin(), path.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == path.end()) {
        result = calc.calculate(SOUTH);

        // hit a wall
        if(result == 0) {
            walls.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
        } 
        // not wall, not oxygen system
        else if(result == 1) {
            path.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
            search_space(new_coordinate, calc);
            calc.calculate(NORTH);
        } 
        // oxygen system
        else if(result == 2) {
            oxygen_station = new_coordinate;
            search_space(new_coordinate, calc);
            calc.calculate(NORTH);
        }
    }

    // next coordinate - west move
    new_coordinate = std::make_pair(start_coordinate.first, start_coordinate.second-1);
    if(std::find(walls.begin(), walls.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == walls.end()
    && std::find(path.begin(), path.end(), std::make_pair(new_coordinate.first, new_coordinate.second)) == path.end()) {
        result = calc.calculate(WEST);

        if(result == 0) {
            walls.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
        } 
        // not wall, not oxygen system
        else if(result == 1) {
            path.push_back(std::make_pair(new_coordinate.first, new_coordinate.second));
            search_space(new_coordinate, calc);
            calc.calculate(EAST);
        } 
        // oxygen system
        else if(result == 2) {
            oxygen_station = new_coordinate;
            search_space(new_coordinate, calc);
            calc.calculate(EAST);
        }
    }

    return;
}

std::vector<std::pair<long long int, long long int>> spreaders;

long long int spread_oxygen() {
    std::vector<std::pair<long long int, long long int>> neighbours;
    std::pair<long long int, long long int> neighbour;
    std::vector<std::pair<long long int, long long int>>::iterator it;
    int index;

    if(path.empty()) {
        return 0;
    }

    std::for_each(spreaders.begin(), spreaders.end(), [&](std::pair<long long int, long long int> pair) {
        neighbour = std::make_pair(pair.first + 1, pair.second);
        it = std::find(path.begin(), path.end(), neighbour);
        if(it != path.end()) {
            neighbours.push_back(neighbour); 
            index = std::distance(path.begin(), it);
            path.erase(path.begin() + index);
        }
        
        neighbour = std::make_pair(pair.first, pair.second + 1);
        it = std::find(path.begin(), path.end(), neighbour);
        if(it != path.end()) {
            neighbours.push_back(neighbour);
            index = std::distance(path.begin(), it);
            path.erase(path.begin() + index);
        }

        neighbour = std::make_pair(pair.first - 1, pair.second);
        it = std::find(path.begin(), path.end(), neighbour);
        if(it != path.end()) {
            neighbours.push_back(neighbour);
            index = std::distance(path.begin(), it);
            path.erase(path.begin() + index);
        }

        neighbour = std::make_pair(pair.first, pair.second - 1);
        it = std::find(path.begin(), path.end(), neighbour);
        if(it != path.end()) {
            neighbours.push_back(neighbour);
            index = std::distance(path.begin(), it);
            path.erase(path.begin() + index);
        }
    });

    spreaders.clear();
    spreaders.insert(spreaders.end(), neighbours.begin(), neighbours.end());

    long long int result = spread_oxygen();
    return result + 1;
}

int main() {
    std::ifstream file("./day15.txt");
    std::string line;
    getline(file, line);
    std::string element;
    std::stringstream ss(line);
   
    std::vector<long long int> elements;
    // put elements to int vector
    while(std::getline(ss, element, ',')) {
        elements.push_back(std::stoll(element));
    }

    std::pair<long long int, long long int> start_position = std::make_pair(0, 0);
    Intcode_calculator calc(elements);

    long long int number_of_steps = search_for_oxygen_system(start_position, calc, 0);
    std::cout << "part1 = " << number_of_steps << std::endl;

    walls.clear();
    path.clear();

    Intcode_calculator calc2(elements);
    path.push_back(start_position);
    search_space(start_position, calc2);

    spreaders.push_back(oxygen_station);

    std::set<std::pair<long long int, long long int>> help;
    help.insert(walls.begin(), walls.end());
    help.insert(path.begin(), path.end());

    long long int y = 100;

    std::for_each(help.begin(), help.end(), [&y](const auto& pair) {

        if(y != pair.first) {
            y = pair.first;
            std::cout << std::endl;
        }

        if(y == oxygen_station.first && pair.second == oxygen_station.second + 1) {
            std::cout << "*" << std::flush;
        }

        if(std::find(walls.begin(), walls.end(), pair) != walls.end()) {
            std::cout << "#" << std::flush;
        } else if (std::find(path.begin(), path.end(), pair) != path.end()) {
            std::cout << "." << std::flush;
        }
    });
    std::cout << std::endl;

    long long int number_of_minutes = spread_oxygen();
    std::cout << "part2 = " << number_of_minutes << std::endl;

    return 0;
}