/**
 * puzzle input - list of reactions
 * every reaction: quantites of input chemicals -> some quantity of output chemical
 * almost every chemical is produced by exactly one reaction, exception:
 * ORE - raw material input to the entire process and is not produced by a reaction
 * how much ORE is needed to collect before one unit of fuel is produced
 * for example : 1 A, 2 B, 3 C => 2 D...10 D is produced by consuming 5 A, 10 B and 15 C
 * */

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <bits/stdc++.h>
#include <unordered_map>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#define FUEL "FUEL"
#define ORE "ORE"

class Chemical {
    public:
        Chemical(std::string _name, int _quantity) : name(_name), quantity(_quantity) {}
        std::string name;
        int quantity;

    private:
};

// key = inputs, value = outout
std::unordered_map<std::string, std::vector<Chemical>> reactions;
std::unordered_map<std::string, int> name_and_quantity;
// fuel reaction
std::vector<Chemical> fuel_reaction;

bool all_elements_ore(std::vector<Chemical>);
int sum_ore(std::vector<Chemical>);
int multiply_index(Chemical);
void replace_reaction_with_inputs(Chemical, int, std::vector<Chemical> &);

int calculate_ore(std::vector<Chemical> &inputs, int ore) {
    if(all_elements_ore(inputs)) {
        return sum_ore(inputs);
    }

    // for each input
    for(int i = 0; i < inputs.size(); i++) {
        // i-th input
        int multiply_factor = multiply_index(inputs[i]);
        replace_reaction_with_inputs(inputs[i], multiply_factor, inputs);

        std::vector<Chemical> next_input;
        next_input.push_back(inputs[i]);
        int result = calculate_ore(next_input, ore);

        ore += result;
        // ore = calculate_ore(inputs);
        inputs[i].name = ORE;
        inputs[i].quantity = ore;
    }

    return ore;
}

int main() {

    std::ifstream file("./day14.txt");
    std::string line;

    while(getline(file, line)) {
        bool fuel_elements = false;
        std::vector<std::string> elements;

        if (line.find(FUEL) != std::string::npos) {
            fuel_elements = true;
        }

        boost::split(elements, line, boost::is_any_of(",=>"));

        std::vector<Chemical> inputs_and_outputs;
        for(int i = 0; i < elements.size(); i++) {
            if(elements[i].empty()) {
                continue;
            }
            boost::algorithm::trim(elements[i]);

            std::string quantity_string = "";
            std::string name = "";

            for(int j = 0; j < elements[i].size(); j++) {
                std::string part = elements[i].substr(j, 1);

                if(isspace(elements[i].at(j))) {
                    continue;
                }

                if(isdigit(elements[i].at(j))) {
                    quantity_string.append(part);
                } else {
                    name.append(part);
                }
            }
            Chemical chemical(name, std::stoi(quantity_string));
            inputs_and_outputs.push_back(chemical);
        }

        std::vector<Chemical> inputs(inputs_and_outputs.begin(), inputs_and_outputs.begin() + inputs_and_outputs.size() - 1);
        Chemical output = inputs_and_outputs[inputs_and_outputs.size()-1];

        if(fuel_elements) {
            fuel_reaction = inputs;
        } else {
            reactions[output.name] = inputs;
            name_and_quantity[output.name] = output.quantity;
        }
    }
    
    int part1 = calculate_ore(fuel_reaction, 0);
    std::cout << "part1 = " << part1 << std::endl;
    return 0;
}

// remove chemical and insert inputs to reaction where chemical is output
void replace_reaction_with_inputs(Chemical chemical, int multiply_factor, std::vector<Chemical> &inputs) {
    if(chemical.name == ORE) {
        return;
    }

    // insert inputs for chemical
    for(auto it = reactions.begin(); it != reactions.end(); it++) {
        if(it->first == chemical.name) {
            std::for_each(it -> second.begin(), it -> second.end(), [&](Chemical c) {
                inputs.push_back(Chemical(c.name, c.quantity * multiply_factor));
            });
            break;
        }
    }

    // remove chemical
    int i;
    for(i = 0; i < inputs.size(); i++) {
        if(inputs[i].name == chemical.name) break;
    }
    inputs.erase(inputs.begin() + i);
}

bool all_elements_ore(std::vector<Chemical> inputs) {
    for(int i = 0; i < inputs.size(); i++) {
        if(inputs[i].name != ORE) return false;
    }
    return true;
}

int sum_ore(std::vector<Chemical> inputs) {
    int count = 0;
    std::for_each(inputs.begin(), inputs.end(), [&](Chemical chemical) {
        count += chemical.quantity;
    }); 

    return count;
}

int multiply_index(Chemical chemical) {
    int multiply_index = 0;
    int new_quantity = chemical.quantity;
    for(auto it : reactions) {
        if(it.first == chemical.name) {
            int old_quantity = name_and_quantity[it.first];
            // check how many reactions need to happen to provide needed amoount of chemical
            if(chemical.quantity / old_quantity == 0) {
                multiply_index = 1;
            } else if(chemical.quantity % old_quantity != 0) {
                multiply_index = (chemical.quantity/old_quantity + 1);
            } else {
                multiply_index = chemical.quantity/old_quantity;
            }
            break;
        }
    }
    return multiply_index;   
}


/**
 * pr. chemical quantity = 1, old_quantity = 3, new_quantity = 3
 * pr. chemical quantity = 4, old_quantity = 3, new_quantity = 2 * 4 = 8
 * 
 * 
 * */