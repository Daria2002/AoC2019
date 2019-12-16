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

void day5(std::vector<long long int>, int);

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
   
    day5(elements, input);

    return 0;
}

void day5(std::vector<long long int> elements, int input) {
    int num_of_params = 1;
    std::string element;
    long long int output;
    std::array<int, 3> indices_mode = {0};
    int i;
    int relative_base = 0;
    std::map<int, std::function<void()>> functions;
   
    functions.emplace(1, [&]() {
        num_of_params = 3;
        std::array<long long int, 3> params;
        for(int j = 0; j < num_of_params; j++) {
            if(indices_mode[j] == 0) {
                params[j] = elements[i+j+1];
            } else if(indices_mode[j] == 1) {
                params[j] = i+j+1;
            } else if(indices_mode[j] == 2) {
                params[j] = elements[i+j+1]+relative_base;
            }
        }
        elements[params[2]] = elements[params[0]] + elements[params[1]];
    });

    functions.emplace(2, [&]() {
        num_of_params = 3;
        std::array<long long int, 3> params;
        for(int j = 0; j < num_of_params; j++) {
            if(indices_mode[j] == 0) {
                params[j] = elements[i+j+1];
            } else if(indices_mode[j] == 1) {
                params[j] = i+j+1;
            } else if(indices_mode[j] == 2) {
                params[j] = elements[i+j+1]+relative_base;
            }
        }
        elements[params[2]] = elements[params[0]] * elements[params[1]];    
    });

    functions.emplace(3, [&]() {
        num_of_params = 1;
        int ind;
        if(indices_mode[2] == 0) {
            ind = elements[i+1];
        } else if(indices_mode[2] == 1) {
            ind = i+1;
        } else if(indices_mode[2] == 2) {
            ind = elements[i+1]+relative_base;
        }
       
        elements[ind] = input;
    });

    functions.emplace(4, [&]() {
        num_of_params = 1;
        if(indices_mode[0] == 0) {
            output = elements[elements[i+1]];
        } else if(indices_mode[0] == 1) {
            output = elements[i+1];
        } else if(indices_mode[0] == 2) {
            if(elements[i+1]+relative_base >= elements.size()) {
                fill_with_zeros();
            }
            output = elements[elements[i+1]+relative_base];
        }
        std::cout << "output = " << output << std::endl;
    });

    functions.emplace(5, [&]() {
        num_of_params = 2;
        std::array<long long int, 2> params;
        for(int j = 0; j < num_of_params; j++) {
            if(indices_mode[j] == 0) {
                params[j] = elements[i+j+1];
            } else if(indices_mode[j] == 1) {
                params[j] = i+j+1;
            } else if(indices_mode[j] == 2) {
                params[j] = elements[i+j+1]+relative_base;
            }
        }
        if(elements[params[0]] != 0) {
            // because there is num_of_params+1 in for loop
            num_of_params = -1;
            i = elements[params[1]];
        }
    });

    functions.emplace(6, [&]() {
         num_of_params = 2;
        std::array<long long int, 2> params;
        for(int j = 0; j < num_of_params; j++) {
            if(indices_mode[j] == 0) {
                params[j] = elements[i+j+1];
            } else if(indices_mode[j] == 1) {
                params[j] = i+j+1;
            } else if(indices_mode[j] == 2) {
                params[j] = elements[i+j+1]+relative_base;
            }
        }
       
        if(elements[params[0]] == 0) {
            // because there is num_of_params+1 in for loop
            num_of_params = -1;
            std::cout << "i je prije bio = " << i << ", a sad je = " << elements[params[1]] << std::endl;
            i = elements[params[1]];
        }
    });

    functions.emplace(7, [&]() {
        num_of_params = 3;
        std::array<long long int, 3> params;
        for(int j = 0; j < num_of_params; j++) {
            if(indices_mode[j] == 0) {
                params[j] = elements[i+j+1];
            } else if(indices_mode[j] == 1) {
                params[j] = i+j+1;
            } else if(indices_mode[j] == 2) {
                params[j] = elements[i+j+1]+relative_base;
            }
        }

        if(elements[params[0]] < elements[params[1]]) {
            elements[params[2]] = 1;
        } else {
            elements[params[2]] = 0;
        }
    });

    functions.emplace(8, [&]() {
        num_of_params = 3;
        std::array<long long int, 3> params;
        for(int j = 0; j < num_of_params; j++) {
            if(indices_mode[j] == 0) {
                params[j] = elements[i+j+1];
            } else if(indices_mode[j] == 1) {
                params[j] = i+j+1;
            } else if(indices_mode[j] == 2) {
                params[j] = elements[i+j+1]+relative_base;
            }
        }

        if(elements[params[0]] == elements[params[1]]) {
            elements[params[2]] = 1;
        } else {
            elements[params[2]] = 0;
        }
    });
   
    functions.emplace(9, [&]() {
        num_of_params = 1;
        int el;
        if(indices_mode[0] == 0) {
            el = elements[elements[i+1]];
        } else if(indices_mode[0] == 1) {
            el = elements[i+1];
        } else if(indices_mode[0] == 2) {
            el = elements[elements[i+1]+relative_base];
        }
       
        relative_base += el;
    });

    for(i = 0; i < elements.size() && elements[i] != 99; i += num_of_params+1) {
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
        functions[element[element.size()-1]-ASCII_ZERO]();
    }
}
