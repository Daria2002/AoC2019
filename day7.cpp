#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <algorithm>

#define ASCII_ZERO 48

int main() {

    std::ifstream file("day7.txt");

    std::string str;
    std::getline(file, str);

    std::array<int, 5> phase_numbers = {0, 1, 2, 3, 4};
    std::sort(phase_numbers.begin(), phase_numbers.end());
    
    int output = 0;

    do {
        
    } while (std::next_permutation(phase_numbers.begin(), phase_numbers.end()));
}

int day5(std::vector<int> elements, int input[]) {
    int num_of_params = 1;
    std::string element;
    int output;
    std::array<int, 3> indices_mode = {0};
    int i;
    int index = 0;
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
        elements[ind] = input[index++];
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
}