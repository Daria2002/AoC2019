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

class Tile {
    public:
        int _x, _y, _z;
        Tile(int x, int y, int z) : _x(x), _y(y), _z(z) { }
};

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
        std::map<int, std::function<int()>> functions;

        Intcode_calculator(std::vector<long long int> _elements) : elements(_elements)
        {
            // capture class members by saying 'this' in the capture list
            functions.emplace(Operations::SUM, [&]() {
                num_of_params = 3;
                elements[params[2]] = elements[params[0]] + elements[params[1]];
                return 0;
            });

            functions.emplace(Operations::MUL, [&]() {
                num_of_params = 3;
                elements[params[2]] = elements[params[0]] * elements[params[1]];
                return 0; 
            });

            functions.emplace(Operations::SAVE_INPUT, [&]() {
                num_of_params = 1;
                std::cin >> input;
                elements[params[0]] = input;
                return 0;
            });

            functions.emplace(Operations::OUTPUT, [&]() {
                num_of_params = 1;
                output = elements[params[0]];
                outputs.push_back(output);
                return output;
            });

            functions.emplace(Operations::JUMP_IF_TRUE, [&]() {
                num_of_params = 2;
                if(elements[params[0]] != 0) {
                    // because there is num_of_params+1 in for loop
                    num_of_params = -1;
                    i = elements[params[1]];
                }
                return 0;
            });

            functions.emplace(Operations::JUMP_IF_FALSE, [&]() {
                num_of_params = 2;
                if(elements[params[0]] == 0) {
                    // because there is num_of_params+1 in for loop
                    num_of_params = -1;
                    i = elements[params[1]];
                }
                return 0;
            });

            functions.emplace(Operations::LESS_THAN, [&]() {
                num_of_params = 3;
                if(elements[params[0]] < elements[params[1]]) {
                    elements[params[2]] = 1;
                } else {
                    elements[params[2]] = 0;
                }
                return 0;
            });

            functions.emplace(Operations::EQUALS, [&]() {
                num_of_params = 3;
                if(elements[params[0]] == elements[params[1]]) {
                    elements[params[2]] = 1;
                } else {
                    elements[params[2]] = 0;
                }
                return 0;
            });
       
            functions.emplace(Operations::RELATIVE_BASE, [&]() {
                num_of_params = 1;
                relative_base += elements[params[0]];
                return 0;
            });
        }

        int calculate() {
            for(i = last_index; i < elements.size() && elements[i] != Operations::HALT; i += num_of_params + 1) {
                // std::cout << "i = " << i << std::endl;
                element = std::to_string(elements[i]);
                int help = 0;
                for(int k = 0; k < 3; k++) {
                    if(element.size() > 4-k) {
                        indices_mode[2-k] = element.at(help++)-ASCII_ZERO;
                    } else {
                        indices_mode[2-k] = 0;
                    }
                }
                //std::cout << "naredba = " << element[element.size()-1]-ASCII_ZERO << std::endl;
               
                for(int j = 0; j < 3; j++) {
                    // last element and it is 99
                    if(i+j+1 >= elements.size()) {
                        params[j] = 0;
                        continue;
                    }
                   
                    if(indices_mode[j] == 0) {
                        params[j] = elements[i + j + 1];
                    } else if(indices_mode[j] == 1) {
                        params[j] = i + j + 1;
                    } else if(indices_mode[j] == 2) {
                        params[j] = elements[i + j + 1]+relative_base;
                    }
                   
                    // if index greater than elements size
                    if(params[j] > elements.size()) {
                        fill_with_zeros(elements, params[j]);
                    }
                }
                if(element[element.size() - 1] - ASCII_ZERO == 4) {
                    int result = functions[element[element.size()-1]-ASCII_ZERO]();
                    last_index = i + num_of_params + 1;
                    return result;
                }

                functions[element[element.size()-1]-ASCII_ZERO]();
            }
            return -1;
        }

        int last_index = 0;
        int get_number_of_block_tiles() {
            int counter = 0;
            for(int i = 2; i < outputs.size(); i = i + 3) {
                if(outputs[i] == 2) {
                    counter++;
                }
            }
            return counter;
        }

    private:
        std::vector<int> outputs;
        std::array<long long int, 3> params;
        int num_of_params = 1;
        int relative_base = 0;
        std::vector<long long int> elements;
        std::array<int, 3> indices_mode = {0};
        std::string element;
        long long int output;
        int i;
        int input;
       
        void fill_with_zeros(std::vector<long long int> &elements, long long int index) {
            int size = elements.size();
            for(int i = size; i <= index; i++) {
                elements.push_back(0);
            }
        }
};

void update_matrix(int x, int y, int tile_id, std::vector<std::vector<char>> matrix) {
    char tile;
    switch (tile_id)
    {
    case 0:
        tile = '.';
        break;
    case 1:
        tile = '|';
        break;
    case 2:
        tile = '#';
        break;
    case 3:
        tile = '_';
        break;
    case 4:
        tile = 'o';
        break;
    default:
        break;
    }
    
    matrix[y][x] = tile;
}

void draw_matrix(std::vector<std::vector<char>> matrix) {
    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix[i].size(); j++) {
            std::cout << matrix[i][j];
        }
        std::cout << " " << std::endl;
    }
}

int max_dimension(std::vector<Tile> tiles, int dimension) {
    // dimension == 0 -> width, dimension == 1 -> height
    int width;
    int height;
    for(int i = 0; i < tiles.size(); i++) {
        if(tiles[i]._x > width) {
            width = tiles[i]._x;
        }
        if(tiles[i]._y > width) {
            width = tiles[i]._y;
        }
    }

    return (dimension == 0 ? width : height);
}

void initialize_matrix(int width, int height, std::vector<std::vector<char>>& matrix) {
    matrix = std::vector<std::vector<char>> (
        height, std::vector<char> (width, '.')
    );
}

int main() {
    int counter = 0;
    bool matrix_initialized = false;
    std::ifstream file("./day13.txt");
    std::string line;
    getline(file, line);
    std::string element;
    std::stringstream ss(line);
   
    std::vector<long long int> elements;
    // put elements to int vector
    while(std::getline(ss, element, ',')) {
        elements.push_back(std::stoll(element));
    }
   
    Intcode_calculator calc(elements);
    std::vector<std::vector<char>> matrix;

    while(calc.last_index < elements.size() && elements[calc.last_index] != 99) {
        if(calc.calculate() == -1) {
            break;
        }
    }

    std::cout << "part1 = " << calc.get_number_of_block_tiles() << std::endl;

    // part2

    // Memory address 0 represents the number of quarters
    // that have been inserted; set it to 2 to play for free.
    elements[0] = 2;
    Intcode_calculator calc2(elements);
    std::vector<Tile> tiles;

    while(calc2.last_index < elements.size() && elements[calc.last_index] != 99) {
        int x = calc2.calculate();
        int y = calc2.calculate();
        int value = calc2.calculate();

        if(x == -1 && y == 0) {
            std::cout << "score = " << value << std::endl;
            if(!matrix_initialized) {
                int width = max_dimension(tiles, 0);
                int height = max_dimension(tiles, 1);
                initialize_matrix(width, height, matrix);
            }
            matrix_initialized = true;
        } else {
            if(matrix_initialized) {
                update_matrix(x, y, value, matrix);
                draw_matrix(matrix);
            } else {
                tiles.push_back(Tile(x, y, value));
            }
        }
    }
    return 0;
}


/*
empty tile : "."
wall tile : "|"
block tile : "#"
horizontal paddle : "_"
ball : "o"
*/