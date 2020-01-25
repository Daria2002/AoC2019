#include <stdio.h>
#include <vector>
#include <ostream>
#include <iostream>
#include <map>
#include <functional>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

// default input is zero, in part1 input is not used
#define DEFAULT_INPUT 0
#define ASCII_ZERO 48

template<class T>
std::ostream& operator<<(std::ostream& stream, const std::pair<T, T>& pair) {
    stream << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    return stream;
}

template<class T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& v) {
    stream << "[";
    for(int i = 0; i < v.size(); i++) {
        stream << v[i];
    }
    stream << "]";
    return stream;
} 

template<class T>
std::ostream& operator<<(std::ostream& stream, const std::vector<std::vector<T>>& v) {
    for(int i = 0; i < v.size(); i++) {
        std::cout << v[i] << std::endl;
    }
}

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
        Intcode_calculator(std::vector<int> _elements) : elements(_elements)
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
                std::cout << "input is used" << std::endl;
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

        int calculate(int _input) {
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
        int last_index;
        std::vector<int> elements;
        std::array<int, 3> params;
        int num_of_params = 1;
        int relative_base = 0;
        std::array<int, 3> indices_mode = {0};
        std::string element;
        int output;
        int i;
        int input;
       
        void fill_with_zeros(std::vector<int> &elements, int index) {
            int size = elements.size();
            for(int i = size; i <= index; i++) {
                elements.push_back(0);
            }
        }
};

std::vector<int> get_input_elements(std::string file_name) {
    std::ifstream file(file_name);
    std::string file_content;
    getline(file, file_content);
    std::stringstream ss(file_content);
    std::vector<int> elements;
    std::string tmp;
    while (getline(ss, tmp, ',')) {
        elements.push_back(std::stoi(tmp));
    }
    return elements;
}

int main() {
    std::vector<int> elements = get_input_elements("./day17.txt");
    Intcode_calculator calc(elements);
    int intcode_result = 0;

    std::vector<std::vector<std::string>> matrix;
    std::vector<std::string> tmp_vector;

    while(intcode_result != -1) {
        intcode_result = calc.calculate(DEFAULT_INPUT);
        switch (intcode_result){
        case 35:
            tmp_vector.push_back("#");
            break;
        case 46:
            tmp_vector.push_back(".");
            break;
        case 10:
            if(tmp_vector.empty()) break;
            matrix.push_back(tmp_vector);
            tmp_vector.clear();
            break;
        default:
            break;
        }
    }

    std::cout << matrix;

}