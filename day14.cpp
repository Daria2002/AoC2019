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
#include <map>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <iterator>

#define FUEL "FUEL"
#define ORE "ORE"

class Chemical {
    public:
        Chemical(std::string _name, int _quantity) : name(_name), quantity(quantity) {}
        std::string name;
        int quantity;
    private:
};

// key = inputs, value = outout
std::map<Chemical, std::vector<Chemical>> reactions;
// fuel reaction
std::vector<Chemical> fuel_reaction;

bool all_elements_ore(std::vector<Chemical>);
int sum_ore(std::vector<Chemical>);
void check_quantity(Chemical, std::vector<Chemical> &);

int calculate_ore(std::vector<Chemical> reaction) {
    if(all_elements_ore(reaction)) {
        return sum_ore(reaction);
    }

    // for each input
    for(int i = 0; i < reaction.size(); i++) {
        // i-th input
        check_quantity(reaction[i], reaction);
        int ore = calculate_ore();
        replace_current_with_ore();
    }
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

        std::vector<Chemical> inputs(inputs_and_outputs.begin(), inputs_and_outputs.end() - 1);
        Chemical output = inputs_and_outputs[inputs_and_outputs.size()-1];

        if(fuel_elements) {
            fuel_reaction = inputs;
        } else {
            reactions[output] = inputs;
        }
    }
    
    int part1 = calculate_ore(fuel_reaction);
    std::cout << "part1 = " << calculate_ore << std::endl;
    return 0;
}

bool all_elements_ore(std::pair<std::vector<Chemical>, Chemical> reaction) {
    for(int i = 0; i < reaction.first.size(); i++) {
        if(reaction.first[i].name != ORE) return false;
    }
    return true;
}

int sum_ore(std::pair<Chemical, std::vector<Chemical>> reaction) {
    int count = 0;
    std::for_each(reaction.second.begin(), reaction.second.end(), [&](Chemical chemical) {
        count += chemical.quantity;
    }); 

    return count;
}

void check_quantity(Chemical chemical, std::pair<Chemical, std::vector<Chemical>> &reaction) {
    // find position where 
    int index;
    for(index = 0; index < reaction.second.size(); index++) {
        if(reaction.second[index].name == chemical.name) break;
    }

    std::map<Chemical, std::vector<Chemical>>::iterator it;
    for(it = reactions.begin(); it != reactions.end(); it++) {
        if(it -> first.name == chemical.name) {
            int new_quantity;
            // check how many reactions need to happen to provide needed amoount of chemical
            if(chemical.quantity / it -> first.quantity == 0) {
                new_quantity = it -> first.quantity; 
            } else if(chemical.quantity % it -> first.quantity != 0) {
                new_quantity = (chemical.quantity/it -> first.quantity + 1) * chemical.quantity;
            } else {
                new_quantity = (chemical.quantity/it -> first.quantity) * chemical.quantity;
            }
            reaction.second[index].quantity = new_quantity;
            break;
        }
    }   
}