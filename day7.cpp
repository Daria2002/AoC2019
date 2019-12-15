#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <algorithm>

#define ASCII_ZERO 48

class Amplifier
{
    public:
        Amplifier(std::vector<int> elements)
        {   
            elements_private = elements;
        }

        bool halt = false;
        std::vector<int> elements_private;

        int get_output(std::array<int, 2> input, bool ini_start) {
            int num_of_params = 1;
            std::string element;
            int output;
            std::array<int, 3> indices_mode = {0};
            int i;
            std::map<int, std::function<int()>> functions;

            functions.emplace(1, [&]() {
                num_of_params = 3;
                std::array<int, 3> params;
                for(int j = 0; j < num_of_params; j++) {
                    params[j] = indices_mode[j] == 0 ? elements_private[i+j+1] : i+j+1;
                }
                elements_private[params[2]] = elements_private[params[0]] + elements_private[params[1]];
                return 0;
            });

            functions.emplace(2, [&]() {
                num_of_params = 3;
                std::array<int, 3> params;
                for(int j = 0; j < num_of_params; j++) {
                    params[j] = indices_mode[j] == 0 ? elements_private[i+j+1] : i+j+1;
                }
                elements_private[params[2]] = elements_private[params[0]] * elements_private[params[1]];  
                return 0;  
            });

            functions.emplace(3, [&]() {
                num_of_params = 1;
                int ind = indices_mode[2] == 0 ? elements_private[i+1] : i+1;
                if(index >= 2) {
                    elements_private[ind] = input[1];
                } else {    
                    elements_private[ind] = input[index++];
                }
                
                return 0;
            });

            functions.emplace(4, [&]() {
                num_of_params = 1;
                output = indices_mode[0] == 0 ? elements_private[elements_private[i+1]] : elements_private[i+1];
                return output;
            });

            functions.emplace(5, [&]() {
                num_of_params = 2;
                std::array<int, 2> params;
                for(int j = 0; j < num_of_params; j++) {
                    params[j] = indices_mode[j] == 0 ? elements_private[i+j+1] : i+j+1;
                }
                if(elements_private[params[0]] != 0) {
                    // because there is num_of_params+1 in for loop
                    num_of_params = -1;
                    i = elements_private[params[1]];
                    start = i;
                }
                return 0;
            });

            functions.emplace(6, [&]() {
                num_of_params = 2;
                std::array<int, 2> params;
                for(int j = 0; j < num_of_params; j++) {
                    params[j] = indices_mode[j] == 0 ? elements_private[i+j+1] : i+j+1;
                }
            
                if(elements_private[params[0]] == 0) {
                    // because there is num_of_params+1 in for loop
                    num_of_params = -1;
                    i = elements_private[params[1]];
                    start = i;
                }
                return 0;
            });

            functions.emplace(7, [&]() {
                num_of_params = 3;
                std::array<int, 3> params;
                for(int j = 0; j < num_of_params; j++) {
                    params[j] = indices_mode[j] == 0 ? elements_private[i+j+1] : i+j+1;
                }

                if(elements_private[params[0]] < elements_private[params[1]]) {
                    elements_private[params[2]] = 1;
                } else {
                    elements_private[params[2]] = 0;
                }
                return 0;
            });

            functions.emplace(8, [&]() {
                num_of_params = 3;
                std::array<int, 3> params;
                for(int j = 0; j < num_of_params; j++) {
                    params[j] = indices_mode[j] == 0 ? elements_private[i+j+1] : i+j+1;
                }

                if(elements_private[params[0]] == elements_private[params[1]]) {
                    elements_private[params[2]] = 1;
                } else {
                    elements_private[params[2]] = 0;
                }
                return 0;
            });

            if(ini_start) {
                start = 0;
                index = 0;
            }

            for(i = start; i < elements_private.size(); i += num_of_params+1) {
                if(elements_private[i] == 99) {
                    halt = true;
                    return output;
                }
                element = std::to_string(elements_private[i]);
                int help = 0;
                for(int k = 0; k < 3; k++) {
                    if(element.size() > 4-k) {
                        indices_mode[2-k] = element.at(help++)-ASCII_ZERO;
                    } else {
                        indices_mode[2-k] = 0;
                    }
                }
                if(element[element.size()-1]-ASCII_ZERO == 4) {
                    start = i+1+1;
                    return functions[element[element.size()-1]-ASCII_ZERO]();
                }
                functions[element[element.size()-1]-ASCII_ZERO]();
            }
        }
    private:
        int start = 0;
        int index = 0;
};

int part1(std::vector<int>);
int part2(std::vector<int>);

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
    std::cout << "part2 = " << part2(elements) << std::endl;

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
            Amplifier a(elements);
            amplifiers.push_back(a);
            input = {phase_numbers[i], output};
            output = amplifiers[i].get_output(input, true);
        }
        outputs.push_back(output);
    } while (std::next_permutation(phase_numbers.begin(), phase_numbers.end()));

    return *std::max_element(outputs.begin(), outputs.end());
}

// phase setting only once
int part2(std::vector<int> elements) {
    std::array<int, 5> phase_numbers = {5, 6, 7, 8, 9};
    std::sort(phase_numbers.begin(), phase_numbers.end());

    int output;
    std::array<int, 2> input;
    std::vector<int> outputs;
    bool halt = false;
    do {
        std::vector<Amplifier> amplifiers;
        output = 0;

        for(int i = 0; i < 5; i++) {
            Amplifier a(elements);
            amplifiers.push_back(a);
        }

        while(!halt) {
            for(int i = 0; i < 5; i++) {
                input = {phase_numbers[i], output};
                output = amplifiers[i].get_output(input, false);
                if(amplifiers[i].halt == true) {
                    halt = true;
                    break;
                }
            }
            outputs.push_back(output);
        }
        halt = false;
    } while (std::next_permutation(phase_numbers.begin(), phase_numbers.end()));

    return *std::max_element(outputs.begin(), outputs.end());
}