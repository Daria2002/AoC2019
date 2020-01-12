#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <functional>
#include <algorithm>
#include <math.h>
#include <utility>
#include <array>

#define ASCII_ZERO 48
#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4

struct Intcode_state {
    int last_index;
    std::vector<long long int> elements;
    int output;
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
        std::map<int, std::function<void(std::vector<long long int>&)>> functions;
        Intcode_calculator()
        {
            // capture class members by saying 'this' in the capture list
            functions.emplace(Operations::SUM, [&](std::vector<long long int>& elements) {
                num_of_params = 3;
                elements[params[2]] = elements[params[0]] + elements[params[1]];
            });

            functions.emplace(Operations::MUL, [&](std::vector<long long int>& elements) {
                num_of_params = 3;
                elements[params[2]] = elements[params[0]] * elements[params[1]];    
            });

            functions.emplace(Operations::SAVE_INPUT, [&](std::vector<long long int>& elements) {
                num_of_params = 1;
                elements[params[0]] = input;
            });

            functions.emplace(Operations::OUTPUT, [&](std::vector<long long int>& elements) {
                num_of_params = 1;
                output = elements[params[0]];
            });

            functions.emplace(Operations::JUMP_IF_TRUE, [&](std::vector<long long int>& elements) {
                num_of_params = 2;
                if(elements[params[0]] != 0) {
                    // because there is num_of_params+1 in for loop
                    num_of_params = -1;
                    i = elements[params[1]];
                }
            });

            functions.emplace(Operations::JUMP_IF_FALSE, [&](std::vector<long long int>& elements) {
                num_of_params = 2;
                if(elements[params[0]] == 0) {
                    // because there is num_of_params+1 in for loop
                    num_of_params = -1;
                    i = elements[params[1]];
                }
            });

            functions.emplace(Operations::LESS_THAN, [&](std::vector<long long int>& elements) {
                num_of_params = 3;
                if(elements[params[0]] < elements[params[1]]) {
                    elements[params[2]] = 1;
                } else {
                    elements[params[2]] = 0;
                }
            });

            functions.emplace(Operations::EQUALS, [&](std::vector<long long int>& elements) {
                num_of_params = 3;
                if(elements[params[0]] == elements[params[1]]) {
                    elements[params[2]] = 1;
                } else {
                    elements[params[2]] = 0;
                }
            });
       
            functions.emplace(Operations::RELATIVE_BASE, [&](std::vector<long long int>& elements) {
                num_of_params = 1;
                relative_base += elements[params[0]];
            });
        }

        Intcode_state calculate(
            int last_index,
            int input_direction,
            std::vector<long long int> elements) {

            input = input_direction;
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

                functions[element[element.size()-1]-ASCII_ZERO](elements);
                if(element[element.size()-1]-ASCII_ZERO == 4) {
                    last_index = i + num_of_params + 1;
                    struct Intcode_state state = {last_index, elements, output};
                    return state;
                }
            }                    
            struct Intcode_state state = {-1, elements, output};
            return state;
        }

    private:
        std::array<long long int, 3> params;
        int num_of_params = 1;
        int relative_base = 0;
        std::array<int, 3> indices_mode = {0};
        std::string element;
        int output;
        int i;
        int input;
       
        void fill_with_zeros(std::vector<long long int> &elements, long long int index) {
            int size = elements.size();
            for(int i = size; i <= index; i++) {
                elements.push_back(0);
            }
        }
};

// key is coordinate (x, y), value is 0 if wall, otherwise 1 
std::unordered_map<std::pair<long long int, long long int>, int> explored_space;

long long int search_for_oxygen_system(
    std::pair<long long int, long long int> start_coordinate,
    Intcode_calculator calc,
    std::vector<long long int> elements,
    int last_index,
    int steps) {

    Intcode_state result;
    std::pair<long long int, long long int> new_coordinate;

    // next coordinate - north move
    new_coordinate = std::make_pair(start_coordinate.first, start_coordinate.second+1);
    result = calc.calculate(last_index, NORTH, elements);
    
    if(result.output == 0) {
        explored_space[new_coordinate] = 0;
        return 0;
    } 
    // not wall, not oxygen system
    else if(result.output == 1) {
        explored_space[new_coordinate] = 1;
        search_for_oxygen_system(new_coordinate, calc, result.elements, result.last_index, steps);
    } 
    // oxygen system
    else if(result.output == 2) {
        return steps + 1;
    }

    // next coordinate - south move
    new_coordinate = std::make_pair(start_coordinate.first, start_coordinate.second-1);
    result = calc.calculate(last_index, SOUTH, elements);
    if(result.output == 0) {
        explored_space[new_coordinate] = 0;
        return 0;
    } 
    // not wall, not oxygen system
    else if(result.output == 1) {
        explored_space[new_coordinate] = 1;
        search_for_oxygen_system(new_coordinate, calc, result.elements, result.last_index, steps);
        } 
    // oxygen system
    else if(result.output == 2) {
        return steps + 1;
    }

    // next coordinate - west move
    new_coordinate = std::make_pair(start_coordinate.first-1, start_coordinate.second);
    result = calc.calculate(last_index, WEST, elements);
    if(result.output == 0) {
        explored_space[new_coordinate] = 0;
        return 0;
    } 
    // not wall, not oxygen system
    else if(result.output == 1) {
        explored_space[new_coordinate] = 1;
        search_for_oxygen_system(new_coordinate, calc, result.elements, result.last_index, steps);
    } 
    // oxygen system
    else if(result.output == 2) {
        return steps + 1;
    }

    // next coordinate - east move
    new_coordinate = std::make_pair(start_coordinate.first+1, start_coordinate.second);
    result = calc.calculate(last_index, EAST, elements);
    if(result.output == 0) {
        explored_space[new_coordinate] = 0;
        return 0;
    } 
    // not wall, not oxygen system
    else if(result.output == 1) {
        explored_space[new_coordinate] = 1;
        search_for_oxygen_system(new_coordinate, calc, result.elements, result.last_index, steps);
    } 
    // oxygen system
    else if(result.output == 2) {
        return steps + 1;
    }
}

int main() {
    int input;
    std::cin >> input;
   
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
    Intcode_calculator calc;
    long long int number_of_steps = search_for_oxygen_system(start_position, calc, 0, 0);

    std::cout << "part1 = " << number_of_steps << std::endl;

    return 0;
}