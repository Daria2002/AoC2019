#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <algorithm>

#define ASCII_ZERO 48

class Amplifier
{
    public:
        Amplifier()
        {

        }

        int get_output(std::vector<int> elements, std::array<int, 2> input) {
            int num_of_params = 1;
            std::string element;
            int output;
            std::array<int, 3> indices_mode = {0};
            int i;
            int index = 0;
            std::map<int, std::function<int()>> functions;

            functions.emplace(1, [&]() {
                num_of_params = 3;
                std::array<int, 3> params;
                for(int j = 0; j < num_of_params; j++) {
                    params[j] = indices_mode[j] == 0 ? elements[i+j+1] : i+j+1;
                }
                elements[params[2]] = elements[params[0]] + elements[params[1]];
                return 0;
            });

            functions.emplace(2, [&]() {
                num_of_params = 3;
                std::array<int, 3> params;
                for(int j = 0; j < num_of_params; j++) {
                    params[j] = indices_mode[j] == 0 ? elements[i+j+1] : i+j+1;
                }
                elements[params[2]] = elements[params[0]] * elements[params[1]];  
                return 0;  
            });

            functions.emplace(3, [&]() {
                num_of_params = 1;
                int ind = indices_mode[2] == 0 ? elements[i+1] : i+1;
                elements[ind] = input[index++];
                return 0;
            });

            functions.emplace(4, [&]() {
                num_of_params = 1;
                output = indices_mode[0] == 0 ? elements[elements[i+1]] : elements[i+1];
                return output;
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
                return 0;
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
                return 0;
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
                return 0;
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
                return 0;
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
                if(element[element.size()-1]-ASCII_ZERO == 4) {
                    return functions[element[element.size()-1]-ASCII_ZERO]();
                }
                functions[element[element.size()-1]-ASCII_ZERO]();
            }
        }
    private:
};

int part1(std::vector<int>);

int main() {

    std::ifstream file("day7.txt");

    std::string str;
    std::getline(file, str);

    std::string element;
    std::stringstream ss(str);
   
    std::vector<int> elements;
    // put elements to int vector
    while(std::getline(ss, element, ',')) {
        elements.push_back(std::stoi(element));
    }

    std::cout << "part1 = " << part1(elements) << std::endl;
    

    return 0;
}

int part1(std::vector<int> elements) {
    std::array<int, 5> phase_numbers = {0, 1, 2, 3, 4};
    std::sort(phase_numbers.begin(), phase_numbers.end());

    int output;
    std::vector<Amplifier> amplifiers;
    std::array<int, 2> input;
    std::vector<int> outputs;
    do {
        output = 0;
        for(int i = 0; i < 5; i++) {
            Amplifier a;
            amplifiers.push_back(a);
            input = {phase_numbers[i], output};
            output = amplifiers[i].get_output(elements, input);
        }
        outputs.push_back(output);
    } while (std::next_permutation(phase_numbers.begin(), phase_numbers.end()));

    return *std::max_element(outputs.begin(), outputs.end());
}