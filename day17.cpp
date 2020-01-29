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
            return -1;
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
        
        // make something like this : R,8,R,8,R,4,R,4,R,8,L,6,L,2,R,4,R,4,R,8,R,8,R,8,L,6,L,2
        std::vector<std::string> get_movements(std::vector<std::vector<char>> matrix) {
            int mode = 1; // mode (horizontal = 1 or vertical = 2)
            int turn = 1; // turn (right = 1 or left = 2)
            int direction = 1; // direction (to right side = 1, to left side = 2, up = 3, down = 4)
            bool end = false;
            int count_steps = 0;
            std::size_t j = 0;
            std::size_t i = 0;
            // while end of whole line
            while(!end) {
                if(mode == 1) { // horizontal walk
                    // while end of straight movement
                    while (true) {
                        if(direction == 1 && i < matrix[j].size()-1 && matrix[j][i++] != '#') {
                            break;
                        } else if(direction == 2 && i > 0 && matrix[j][i--] != '#') {
                            break;
                        } else if(direction == 3 && j > 0 && matrix[j--][i] != '#') {
                            break;
                        } else if(direction == 4 && j < matrix.size()-1 && matrix[j++][i] != '#') {
                            break;
                        } else {
                            count_steps++;
                        }
                    }
                } else { // vertical walk
                    
                }   
                inputs.push_back(turn == 1 ? 'R' : 'L');
                inputs.push_back(',');
                inputs.push_back(count_steps);
                count_steps = 0;

                if(mode == 1) { // horizontal
                    if(direction == 1) { // to right
                        if(j < matrix.size()-1 && matrix[j+1][i] == '#') { // check right
                            j++;
                            mode = 2;
                            direction = 4;
                            continue;    
                        } else if(j > 0 && matrix[j-1][i] == '#') { // check left
                            j--;
                            mode = 2;
                            direction = 3;
                            continue;
                        } else {
                            break;
                        }
                    } else { // to left
                        if(j > 0 && matrix[j-1][i] == '#') { // check right
                            j--;
                            mode = 2;
                            direction = 3;
                            continue;
                        } else if(j < matrix.size()-1 && matrix[j+1][i] == '#') { // check left
                            j++;
                            mode = 2;
                            direction = 4;
                        } else {
                            break;
                        }
                    }
                } else { // vertical
                    if(direction == 3) { // up
                        if(i > ) { // check left

                        } else if() { // check right

                        } else {
                            break;
                        }
                    } else { // down
                        if() { // check left

                        } else if() { // check right

                        } else {
                            break;
                        }
                    }
                }
            }

            std::for_each(matrix.begin() + start_coordinate.first, matrix.end(), [&] (const auto &row) {
                std::for_each(row.begin() + start_coordinate.second, row.end(), [&] (const auto &el) {
                    if(mode == 1) {
                        if(matrix[j][k+1] == '#') {
                            // while can go straight
                            int count_moves = 0;
                            while(count_moves < 10) {
                                count_moves++;
                            }
                            // push back direction
                            inputs.push_back('R');
                            // push back comma
                            inputs.push_back(',');
                            // push back number of steps
                            inputs.push_back(count_moves + '0');
                        } else if(matrix[j][k+1] != '#' && matrix[j][k-1] == '#') {
                            
                        } else {    
                            // end

                        }
                        k++;
                    }
                });
                if(mode == 1) {
                    j++;
                }
            });
        }

        // return inputs
        std::vector<int> get_routine(std::vector<std::vector<char>> matrix) {
            std::vector<int> inputs;
            std::vector<std::string> moves = get_movements(matrix);

            for(int k = 0; k < moves.size(); k++) {
                
            }

            return inputs;
        }

        void fill_with_zeros(std::vector<int> &elements, int index) {
            int size = elements.size();
            for(std::size_t i = size; i <= index; i++) {
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

std::vector<std::vector<char>> build_map(Intcode_calculator &calc, std::vector<int> elements) {
    int intcode_result = 0;
    std::vector<std::vector<char>> matrix;
    std::vector<char> tmp_vector;
    int j = 0, i = 0;
    while(intcode_result != -1) {
        intcode_result = calc.calculate(DEFAULT_INPUT, matrix);
        char intcode_result_char = static_cast<char>(intcode_result);
        if(intcode_result_char == '^') {
            start_coordinate = std::make_pair(j, i);
        }
        std::cout << intcode_result_char;
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
    // elements[0] = 2;
    // Intcode_calculator calc2(elements);
    // matrix = build_map(calc2, elements);
}