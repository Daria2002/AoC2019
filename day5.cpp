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
    std::array<int, 3> indices = {0};
    int i;

    std::map<int, std::function<void()>> functions;
    functions.emplace(1, [&]() {
        num_of_params = 3;
        elements[indices[2]] = elements[indices[0]] + elements[indices[1]];
    });

    functions.emplace(2, [&]() {
        num_of_params = 3;
        elements[indices[2]] = elements[indices[0]] * elements[indices[1]];
    });

    functions.emplace(3, [&]() {
        num_of_params = 1;
        elements[elements[i+1]] = input;
    });

    functions.emplace(4, [&]() {
        num_of_params = 1;
        output = elements[i+1];
        std::cout << "output = " << output << std::endl;
    });

    for(i = 0; i < elements.size() && elements[i] != 99; i += num_of_params+1) {
        element = std::to_string(elements[i]);
        //!!!!!!
        // 104 valja
        // if there is no elements before opcode, than initialize modes to 0
        if(element.size() != 4) {
            indices = {elements[i+1], elements[i+2], elements[i+3]};
            std::cout << "el val = " << elements[i] << std::endl;
            functions[elements[i]]();
        } else {
            // if position mode use index written in param value, otherwise 
            // param index
            for(int j = 0; j < 3; j++) {
                indices[j] = element.at(j) == 0 ? elements[i+j+1] : i+j+1;
            }
            std::cout << "int val of el.at(3) = " << element.at(3)-ASCII_ZERO << std::endl;
            functions[element.at(3)-ASCII_ZERO]();
        }

    }
    
    return 0;
}