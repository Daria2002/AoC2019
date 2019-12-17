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
        std::map<int, std::function<void(std::array<long long int, 3>)>> functions;

        Intcode_calculator(std::vector<long long int> _elements, int _input) : elements(_elements), input(_input)
        {
            functions.emplace(Operations::SUM, [&](std::array<long long int, 3> param) {
                std::cout << "1" << std::endl;
                num_of_params = 3;
                elements[param[2]] = elements[param[0]] + elements[param[1]];
            });

            functions.emplace(Operations::MUL, [&](std::array<long long int, 3> param) {
                std::cout << "2" << std::endl;
                num_of_params = 3;
                elements[param[2]] = elements[param[0]] * elements[param[1]];    
            });

            functions.emplace(Operations::SAVE_INPUT, [&](std::array<long long int, 3> param) {
                std::cout << "3" << std::endl;
                num_of_params = 1;
               
                //int ind;
                //if(indices_mode[2] == 0) {
                  //  ind = elements[i+1];
                //} else if(indices_mode[2] == 1) {
                  //  ind = i+1;
                //} else if(indices_mode[2] == 2) {
                 //   ind = elements[i+1]+relative_base;
                //}
                //elements[ind] = input;
               
                elements[param[0]] = input;
            });

            functions.emplace(Operations::OUTPUT, [&](std::array<long long int, 3> param) {
                std::cout << "4" << std::endl;
                num_of_params = 1;
                output = elements[param[0]];
                std::cout << "output = " << output << std::endl;
            });

            functions.emplace(Operations::JUMP_IF_TRUE, [&](std::array<long long int, 3> param) {
                std::cout << "5" << std::endl;
                num_of_params = 2;
                if(elements[param[0]] != 0) {
                    // because there is num_of_params+1 in for loop
                    num_of_params = -1;
                    i = elements[param[1]];
                }
            });

            functions.emplace(Operations::JUMP_IF_FALSE, [&](std::array<long long int, 3> param) {
                std::cout << "6" << std::endl;
                num_of_params = 2;
                if(elements[param[0]] == 0) {
                    // because there is num_of_params+1 in for loop
                    num_of_params = -1;
                    std::cout << "i je prije bio = " << i << ", a sad je = " << elements[param[1]] << std::endl;
                    i = elements[param[1]];
                }
            });

            functions.emplace(Operations::LESS_THAN, [&](std::array<long long int, 3> param) {
                std::cout << "7" << std::endl;
                num_of_params = 3;
                if(elements[param[0]] < elements[param[1]]) {
                    elements[param[2]] = 1;
                } else {
                    elements[param[2]] = 0;
                }
            });

            functions.emplace(Operations::EQUALS, [&](std::array<long long int, 3> param) {
                std::cout << "8" << std::endl;
                num_of_params = 3;
                if(elements[param[0]] == elements[param[1]]) {
                    elements[param[2]] = 1;
                } else {
                    elements[param[2]] = 0;
                }
            });
       
            functions.emplace(Operations::RELATIVE_BASE, [&](std::array<long long int, 3> param) {
                std::cout << "9" << std::endl;
                std::cout << "Sada je operacija 9 " << std::endl;
                num_of_params = 1;
                std::cout << "params[0] = " << param[0] << std::endl;
                relative_base += elements[param[0]];
            });
        }

        void calculate() {
            std::array<long long int, 3> params;
            for(i = 0; i < elements.size() && elements[i] != Operations::HALT; i += num_of_params+1) {
                element = std::to_string(elements[i]);
                std::cout << "element = " << element << std::endl;
                int help = 0;
                for(int k = 0; k < 3; k++) {
                    if(element.size() > 4-k) {
                        indices_mode[2-k] = element.at(help++)-ASCII_ZERO;
                    } else {
                        indices_mode[2-k] = 0;
                    }
                }
                std::cout << "naredba = " << element[element.size()-1]-ASCII_ZERO << std::endl;
               
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
                std::cout << "element = " << element << std::endl;
                std::cout << "index = " << element[element.size()-1]-ASCII_ZERO << std::endl;
                std::cout << "params[0] = " << params[0] << std::endl;
                std::cout << "params[1] = " << params[1] << std::endl;
                std::cout << "params[2] = " << params[2] << std::endl;
               
                std::cout << "func size = " << functions.size() << std::endl;
                functions[element[element.size()-1]-ASCII_ZERO](params);
            }
        }

    private:
        int num_of_params = 1;
        int relative_base = 0;
        std::vector<long long int> elements;
        std::array<int, 3> indices_mode = {0};
        std::string element;
        long long int output;
        int i;
        int input;
       
        void fill_with_zeros(std::vector<long long int> &elements, long long int index) {
            //std::for_each(elements.end(), elements.end() + index, [](long long int &num) {num = 0;});
            int size = elements.size();
            for(int i = size; i <= index; i++) {
                elements[i] = 0;
            }
        }
};

int main() {
    int input;
    std::cin >> input;
   
    std::ifstream file("./day9.txt");
    std::string line;
    getline(file, line);
    std::string element;
    std::stringstream ss(line);
   
    std::vector<long long int> elements;
    // put elements to int vector
    while(std::getline(ss, element, ',')) {
        elements.push_back(std::stoll(element));
    }
   
    Intcode_calculator calc(elements, input);
    calc.calculate();

    return 0;
}