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
#include <list>
#include <algorithm>
#include <math.h>
#include <utility>
#include <array>

#define ASCII_ZERO 48

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
        std::map<std::pair<int, int>, int> color_map;

        Intcode_calculator(std::vector<long long int> _elements) : elements(_elements)
        {
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
               
                if(color_map.find(current_position) != color_map.end()) {
                    input = color_map[current_position];
                } else {
                    input = 0;
                    // if(start_color_initialized) {
                    //     input = 0;
                    // } else {
                    //     input = start_color;
                    //     start_color_initialized = true;
                    // }
                }
               
                elements[params[0]] = input;
            });
           
            functions.emplace(Operations::OUTPUT, [&]() {
                num_of_params = 1;
                output = elements[params[0]];
                if(color) {
                    color_map[current_position] = output;
                    color = false;
                } else {
                    color = true;
                    int x = current_position.second;
                    int y = current_position.first;
                   
                    if(output == 0) {
                        // turn left
                        if(current_direction == 0) {
                            current_position = std::make_pair(y, x-1);
                        } else if(current_direction == 1) {
                            current_position = std::make_pair(y-1,x);
                        } else if(current_direction == 2) {
                            current_position = std::make_pair(y, x+1);
                        } else if(current_direction == 3) {
                            current_position = std::make_pair(y+1, x);
                        }
                        current_direction = current_direction-1 < 0 ? 3 : current_direction-1;
                    } else if(output == 1) {
                        // turn right
                        if(current_direction == 0) {
                            current_position = std::make_pair(y, x+1);
                        } else if(current_direction == 1) {
                            current_position = std::make_pair(y+1, x);
                        } else if(current_direction == 2) {
                            current_position = std::make_pair(y, x-1);
                        } else if(current_direction == 3) {
                            current_position = std::make_pair(y-1, x);
                        }
                        current_direction = (current_direction + 1) % 4;
                    }
                }
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

        void calculate(int _start_color) {
            color_map[std::make_pair(0, 0)] = start_color;
            color_map.empty();
            current_position = std::make_pair(0, 0);
            start_color = _start_color;
            for(i = 0; i < elements.size() && elements[i] != Operations::HALT; i += num_of_params+1) {
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
            }
        }

        int paint_panels(int _start_color) {
            if(color_map.size() == 0 || start_color != _start_color) {
                calculate(_start_color);
            }
            return color_map.size();
        }

    private:
        std::array<long long int, 3> params;
        int num_of_params = 1;
        int relative_base = 0;
        std::vector<long long int> elements;
        std::array<int, 3> indices_mode = {0};
        std::string element;
        long long int output;
        int i;
        bool start_color_initialized;
        int start_color;
        int input;
        std::pair<int, int> current_position;

        // 0 - up, 1 - right, 2 - down, 3 - left
        int current_direction = 0;
        bool color = true;

        void fill_with_zeros(std::vector<long long int> &elements, long long int index) {
            int size = elements.size();
            for(int i = size; i <= index; i++) {
                elements.push_back(0);
            }
        }
};

int main() {
    std::ifstream file("./day11.txt");
    std::string line;
    getline(file, line);
    std::string element;
    std::stringstream ss(line);
   
    std::vector<long long int> elements;
    // put elements to int vector
    while(std::getline(ss, element, ',')) {
        elements.push_back(std::stoll(element));
    }
    
    int start_color = 0;

    Intcode_calculator calc(elements);
    // int part1 = calc1.paint_panels(start_color);
    // std::cout << "part1 = " << part1 << std::endl;

    std::cout << "part2" << std::endl;
    start_color = 1;
    calc.paint_panels(start_color);
    std::map<std::pair<int, int>, int> color_map = calc.color_map;

    int row_min;
    int row_max;
    int column_min;
    int column_max;

    // get min, get max
    std::for_each(color_map.begin(), color_map.end(), [&](std::pair<std::pair<int, int>, int> pair) {
        // comparing row
        if(pair.first.first > row_max) {
            row_max = pair.first.first;
        }
        if(pair.first.first < row_min) {
            row_min = pair.first.first;
        }
        // comparing columns
        if(pair.first.second > column_max) {
            column_max = pair.first.second;
        }
        if(pair.first.second < column_min) {
            column_min = pair.first.second;
        }
    });

    // row, vector of values in row
    std::vector<std::vector<char>> matrix;
    for(int i = row_min; i <= row_max; i++) {
        std::vector<char> help;
        for(int j = column_min; j <= column_max; j++) {
            help.push_back('.');
        }
        matrix.push_back(help);
    }

    std::for_each(color_map.begin(), color_map.end(), [&](std::pair<std::pair<int, int>, int> pair) {
        // if current panel is white, update matrix
        if(pair.second == 1) {
            matrix[pair.first.first-row_min][pair.first.second-column_min] = '#';
        }
    });

    for(int i = row_min; i <= row_max; i++) {
        for(int j = column_min; j <= column_max; j++) {
            std::cout << matrix[i-row_min][j-column_min];
        }
        std::cout << "" << std::endl;
    }

    return 0;
}
