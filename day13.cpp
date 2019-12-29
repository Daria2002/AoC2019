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
        int _x, _y, _value;
        Tile(int x, int y, int value) : _x(x), _y(y), _value(value) { }
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
                if(_x_ball < _x_tile) {
                    input = -1;
                } else if(_x_ball > _x_tile) {
                    input = 1;
                } else {
                    input = 0;
                }
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

        int calculate(int x_ball, int x_tile) {
            _x_ball = x_ball;
            _x_tile = x_tile;
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
                   
                    std::cout << "Prije fill " << std::endl;
                    // if index greater than elements size
                    if(params[j] > elements.size()) {
                        fill_with_zeros(elements, params[j]);
                    }
                    std::cout << "poslje fill" << std::endl;
                }

                std::cout << "funkcija = " << element[element.size() - 1] - ASCII_ZERO << std::endl;

                if(element[element.size() - 1] - ASCII_ZERO == 4) {
                    int result = functions[element[element.size()-1]-ASCII_ZERO]();
                    last_index = i + num_of_params + 1;
                    return result;
                }
                functions[element[element.size()-1]-ASCII_ZERO]();
            }
            return -2;
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
        int _x_ball;
        int _x_tile;
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

void update_matrix(int x, int y, int tile_id, std::vector<std::vector<char>>& matrix, 
int &x_ball, int &x_tile) {
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
        x_tile = x;
        break;
    case 4:
        tile = 'o';
        x_ball = x;
        break;
    default:
        break;
    }
    
    matrix[y][x] = tile;
}

// zasto kada se salje by ref radi, a kad se šalje by value ne radi nego se pojavi
// error : corrupted size vs. prev_size
void draw_matrix(std::vector<std::vector<char>>& matrix) {
    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix[i].size(); j++) {
            std::cout << matrix[i][j];
        }
        std::cout << " " << std::endl;
    }
}

int max_dimension(std::vector<Tile> tiles, int dimension) {
    // dimension == 0 -> width, dimension == 1 -> height
    int width = 0;
    int height = 0;
    for(int i = 0; i < tiles.size(); i++) {
        if(tiles[i]._x > width) {
            // tiles[i]._x has values in range from [0, width-1]
            width = tiles[i]._x + 1;
        }
        if(tiles[i]._y > height) {
            height = tiles[i]._y + 1;
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
    int score;
   
    std::vector<long long int> elements;
    // put elements to int vector
    while(std::getline(ss, element, ',')) {
        elements.push_back(std::stoll(element));
    }
   
    Intcode_calculator calc(elements);
    std::vector<std::vector<char>> matrix;

    while(calc.last_index < elements.size() && elements[calc.last_index] != 99) {
        if(calc.calculate(0, 0) == -2) {
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
    int x, y, value;
    int x_ball = 0;
    int x_tile = 0;

    while(calc2.last_index < elements.size() && elements[calc.last_index] != 99) {
        std::cout << "prije x " << std::endl;
        x = calc2.calculate(x_ball, x_tile);
        if(x == -2) {
            break;
        }
        std::cout << "poslje x " << std::endl;
        y = calc2.calculate(x_ball, x_tile);
        if(y == -2) {
            break;
        }
        std::cout << "poslje y " << std::endl;
        value = calc2.calculate(x_ball, x_tile);
        if(value == -2) {
            break;
        }

        if(x == -1 && y == 0) {
            score = value;
            std::cout << "score = " << score << std::endl;
            if(!matrix_initialized) {
                int width = max_dimension(tiles, 0);
                int height = max_dimension(tiles, 1);

                initialize_matrix(width, height, matrix);
                for(int i = 0; i < tiles.size(); i++) {
                    std::cout << "i = " << i << std::endl;
                    update_matrix(tiles[i]._x, tiles[i]._y, tiles[i]._value, matrix, x_ball, x_tile);
                }
                std::cout << "nakon update " << std::endl;
                matrix_initialized = true;
                std::cout << "prije crtanja " << std::endl;
                draw_matrix(matrix);
                std::cout << "nacrtano" << std::endl;
            }
        } else {
            if(matrix_initialized) {
                std::cout << "update1 " << std::endl;
                try
                {
                    update_matrix(x, y, value, matrix, x_ball, x_tile);
                }
                catch(const std::exception& e) {}
                
                std::cout << "draw1" << std::endl;
                draw_matrix(matrix);
                std::cout << "after draw1"<< std::endl;
            } else {
                tiles.push_back(Tile(x, y, value));
            }
        }
    }
    std::cout << "score = " << score << std::endl;
    return 0;
}


/*
empty tile : "."
wall tile : "|"
block tile : "#"
horizontal paddle : "_"
ball : "o"
*/