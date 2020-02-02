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
#include <stdlib.h>
#include <unordered_map>

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
        stream << v[i] << std::endl;
    }
    return stream;
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
        Intcode_calculator(std::vector<int> _elements, std::pair<int, int> &_start_coordinate)
         : elements(_elements), start_coordinate(_start_coordinate)
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
                if(not_processed) {
                    inputs = get_routine(matrix);
                    not_processed = true;
                }
                input = inputs[num_of_input];
                num_of_input++;
                std::cout << "input funkcija " << std::endl;
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

        int calculate(int _input, std::vector<std::vector<char>> _matrix) {
            matrix = _matrix;
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
            return 0;
        }
        
        // input : map with # and .
        // output : R,8,R,8,R,4,R,4,R,8,L,6,L,2,R,4,R,4,R,8,R,8,R,8,L,6,L,2
        std::vector<char> get_movements(std::vector<std::vector<char>> matrix) {
            std::vector<char> movements;
            int direction = 0; // 0-up, 1-right, 2-down, 3-left
            int j = start_coordinate.first, i = start_coordinate.second, counter = 0;

            while(true) {
                while(can_go_straight(matrix, i, j, direction)) {
                    counter++;
                }
                if(counter > 0) {
                    std::cout << "counter = " << counter << std::endl;
                    movements.push_back(counter + '0');
                    counter = 0;
                }

                if(go_left(matrix, i, j, direction)) {
                     direction = abs(direction - 1) % 4;
                     movements.push_back('L');
                     std::cout << "skrece lijevo" << std::endl;
                     movements.push_back(',');
                } else if(go_right(matrix, i, j, direction)) {
                     direction = (direction + 1) % 4;
                     movements.push_back('R');
                     std::cout << "skrece desno" << std::endl;
                     movements.push_back(',');
                } else {
                    break;
                }
            }
            return movements;
        }

    private:
        bool not_processed = false;
        int num_of_input = 0;
        std::vector<std::vector<char>> matrix;
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
        std::vector<int> inputs;
        std::pair<int, int> start_coordinate;

        bool go_left(std::vector<std::vector<char>> matrix, int i, int j, int dir) {
            if(dir == 0 && i > 0) {
                return matrix[j][i-1] == '#';
            } else if(dir == 1 && j > 0) {
                return matrix[j-1][i] == '#';
            } else if(dir == 2 && i+1 < matrix[j].size()-1) {
                return matrix[j][i+1] == '#';
            } else if(dir == 3 && j < matrix.size()-1) {
                return matrix[j+1][i] == '#';
            }
            return false;
        }

        bool go_right(std::vector<std::vector<char>> matrix, int i, int j, int dir) {
            if(dir == 0 && i > 0) {
                return matrix[j][i+1] == '#';
            } else if(dir == 1 && j > 0) {
                return matrix[j+1][i] == '#';
            } else if(dir == 2 && i+1 < matrix[j].size()-1) {
                return matrix[j][i-1] == '#';
            } else if(dir == 3 && j > 0) {
                return matrix[j-1][i] == '#';
            }
            return false;
        }

        bool can_go_straight(std::vector<std::vector<char>> matrix, int &i, int &j, int direction) {
            if(direction == 0 && j > 0 && matrix[j-1][i] == '#') {
                j--;
                return true;
            } else if(direction == 1 && i < matrix[j].size()-1 && matrix[j][i+1] == '#') {
                i++;
                return true;
            } else if(direction == 2 && j < matrix.size()-1 && matrix[j+1][i] == '#') {
                j++;
                return true;
            } else if(direction == 3 && i > 0 && matrix[j][i-1] == '#') {    
                i--;
                return true;
            }
            return false;
        }

        // input : R,8,R,8,R,4,R,4,R,8,L,6,L,2,R,4,R,4,R,8,R,8,R,8,L,6,L,2
        // output : ascii table value of(a, b, c, a, r, 2, l, 5, r, 6, l, 3)
        std::vector<int> convert_movements_to_inputs(std::vector<char> moves, 
        std::vector<std::vector<char>> matrix) {
            std::vector<int> inputs;
            return inputs;
        }

        // return inputs
        std::vector<int> get_routine(std::vector<std::vector<char>> matrix) {
            std::vector<char> moves = get_movements(matrix); // i.e moves =  R,8,R,8,R,4
            std::vector<int> inputs = convert_movements_to_inputs(moves, matrix);
            return inputs;
        }

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

std::pair<int, int> start_coordinate;

inline bool is_intcode_result_ok(int intcode_result) {
    return (intcode_result == 35 || intcode_result == 46 || intcode_result == 10 || intcode_result == 94);
}

std::vector<std::vector<char>> build_map(Intcode_calculator &calc, std::vector<int> elements) {
    int intcode_result = 10;
    std::vector<std::vector<char>> matrix;
    std::vector<char> tmp_vector;
    int j = 0, i = 0;
    while(is_intcode_result_ok(intcode_result)) {
        intcode_result = calc.calculate(DEFAULT_INPUT, matrix);
        char intcode_result_char = static_cast<char>(intcode_result);
        if(intcode_result_char == '^') {
            start_coordinate = std::make_pair(j, i);
        }
        if(is_intcode_result_ok(intcode_result)) std::cout << intcode_result_char;
        switch (intcode_result){
        case 10:
            if(tmp_vector.empty()) break;
            matrix.push_back(tmp_vector);
            j++;
            i = 0;
            tmp_vector.clear();
            break;
        default:
            tmp_vector.push_back(intcode_result_char);
            i++;
            break;
        }
    }
    return matrix;
}

bool check_neighbours(int i, int j, std::vector<std::vector<char>> matrix) {
    if(j-1 >= 0 && j+1 < matrix[i].size() && i-1 >= 0 && i+1 < matrix.size()) {
        if(matrix[i-1][j] == '#' && matrix[i+1][j] == '#' && matrix[i][j+1] == '#' && matrix[i][j-1] == '#') {
            return true;
        }
    }
    return false;
}

std::vector<std::pair<int, int>> get_intersections(std::vector<std::vector<char>> matrix) {
    std::vector<std::pair<int, int>> intersections;
    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix[i].size(); j++) {
            if(matrix[i][j] == '#' && check_neighbours(i, j, matrix)) {
                intersections.push_back(std::make_pair(i, j));
            }
        }
    }
    return intersections;
}

inline long long int get_alignment_parameter(std::pair<int, int> element) {
    return element.first * element.second;
}

long long int get_sum_of_alignment_params(std::vector<std::pair<int, int>> intersections) {
    long long int sum = 0;
    std::for_each(intersections.begin(), intersections.end(), [&sum](const auto& el) {
        sum += get_alignment_parameter(el);
    });
    return sum;
}

int main() {
    std::vector<int> elements = get_input_elements("./day17.txt");
    Intcode_calculator calc(elements, start_coordinate);
    std::vector<std::vector<char>> matrix = build_map(calc, elements);
    std::vector<std::pair<int, int>> intersections = get_intersections(matrix);
    long long int sum_of_alignment_params = get_sum_of_alignment_params(intersections);
    std::cout << "part1 = " << sum_of_alignment_params << std::endl;


    // part2: change value at position 0 from 1 to 2
    elements[0] = 2;
    Intcode_calculator calc2(elements, start_coordinate);
    matrix = build_map(calc2, elements);
    std::vector<char> v = calc2.get_movements(matrix);
    std::cout << v << std::endl;
}