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
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <array>

#define ASCII_ZERO 48

int main() {
    int input;
    int output;
    std::cin >> input;
   
    std::ifstream file("./day5.txt");
    std::string line;
    getline(file, line);
    std::string element;
    std::stringstream ss(line);
   
    std::vector<int> elements;
    // put elements to int vector
    while(std::getline(ss, element, ',')) {
        elements.push_back(std::stoi(element));
    }
   
    int num_of_params = 1;
    std::array<int, 3> indices_mode = {0};
    int i;

    std::map<int, std::function<void()>> functions;
   
    functions.emplace(1, [&]() {
        num_of_params = 3;
        std::array<int, 3> params;
        for(int j = 0; j < num_of_params; j++) {
            params[j] = indices_mode[j] == 0 ? elements[i+j+1] : i+j+1;
        }
        elements[params[2]] = elements[params[0]] + elements[params[1]];
    });

    functions.emplace(2, [&]() {
        num_of_params = 3;
        std::array<int, 3> params;
        for(int j = 0; j < num_of_params; j++) {
            params[j] = indices_mode[j] == 0 ? elements[i+j+1] : i+j+1;
        }
        elements[params[2]] = elements[params[0]] * elements[params[1]];    
    });

    functions.emplace(3, [&]() {
        num_of_params = 1;
        int ind = indices_mode[2] == 0 ? elements[i+1] : i+1;
        elements[ind] = input;
    });

    functions.emplace(4, [&]() {
        num_of_params = 1;
        output = indices_mode[0] == 0 ? elements[elements[i+1]] : elements[i+1];
        std::cout << "output = " << output << std::endl;
    });

    functions.emplace(5, [&]() {
        num_of_params = 2;
        std::array<int, 2> params;
        for(int j = 0; j < num_of_params; j++) {
            params[j] = indices_mode[j] == 0 ? elements[i+j+1] : i+j+1;
        }
        if(elements[params[0]] != 0) {
            // because there is num_of_params+1 in for loop
            num_of_params = -1;
            i = elements[params[1]];
        }
    });

    functions.emplace(6, [&]() {
         num_of_params = 2;
        std::array<int, 2> params;
        for(int j = 0; j < num_of_params; j++) {
            params[j] = indices_mode[j] == 0 ? elements[i+j+1] : i+j+1;
        }
       
        if(elements[params[0]] == 0) {
            // because there is num_of_params+1 in for loop
            num_of_params = -1;
            i = elements[params[1]];
        }
    });

    functions.emplace(7, [&]() {
        num_of_params = 3;
        std::array<int, 3> params;
        for(int j = 0; j < num_of_params; j++) {
            params[j] = indices_mode[j] == 0 ? elements[i+j+1] : i+j+1;
        }

        if(elements[params[0]] < elements[params[1]]) {
            elements[params[2]] = 1;
        } else {
            elements[params[2]] = 0;
        }
    });

    functions.emplace(8, [&]() {
        num_of_params = 3;
        std::array<int, 3> params;
        for(int j = 0; j < num_of_params; j++) {
            params[j] = indices_mode[j] == 0 ? elements[i+j+1] : i+j+1;
        }

        if(elements[params[0]] == elements[params[1]]) {
            elements[params[2]] = 1;
        } else {
            elements[params[2]] = 0;
        }
    });

    for(i = 0; i < elements.size() && elements[i] != 99; i += num_of_params+1) {
        element = std::to_string(elements[i]);
        int help = 0;
        for(int k = 0; k < 3; k++) {
            if(element.size() > 4-k) {
                indices_mode[2-k] = element.at(help++)-ASCII_ZERO;
            } else {
                indices_mode[2-k] = 0;
            }
        }
        functions[element[element.size()-1]-ASCII_ZERO]();
    }
    return 0;
}