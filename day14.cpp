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

        bool operator == (const Chemical &other) const {
            return (name == other.name && quantity == other.quantity);
        }

    private:
};

struct DataHasher
{
  std::size_t operator () (const std::string &key) const 
  {
    std::size_t seed = 0;
    // boost::hash_combine(seed, boost::hash_value(key.name));
    // boost::hash_combine(seed, boost::hash_value(key.quantity));
    boost::hash_combine(seed, boost::hash_value(key));
    return seed;
  }
};

struct DataHasher2
{
  std::size_t operator () (const Chemical &key) const 
  {
    std::size_t seed = 0;
    // // boost::hash_combine(seed, boost::hash_value(key.name));
    // // boost::hash_combine(seed, boost::hash_value(key.quantity));
    return seed;
  }
};

// key = inputs, value = outout
/**
 * To be able to use std::unordered_map (or one of the other unordered associative containers) with a user-defined key-type, you need to define two things:
 * A Hash function -  this must be a class that overrides operator() and calculates the hash value given an object of the key-type. One particularly 
 *                    straight-forward way of doing this is to specialize the std::hash template for your key-type.
 * A comparison function for equality
*/
typedef std::unordered_map<std::string, std::vector<Chemical>, DataHasher> DataHashMap;
DataHashMap reactions;
std::map<std::string, int> reactions_name_quantity;
std::vector<Chemical> fuel_inputs;
std::vector<Chemical> final_elements;
typedef std::unordered_map<Chemical, int, DataHasher2> OccurrencesMap;
OccurrencesMap final_elements_occurrences_map;

int count_occurrence(Chemical output, Chemical input, int counter) {

    if(output.name == input.name) {
        return output.quantity;
    }

    std::vector<Chemical> inputs = reactions[output.name];

    for(int i = 0; i < inputs.size(); i++) {
        counter += count_occurrence(inputs[i], input, 0);
    }

}

bool is_final_element(Chemical element) {
    for(int i = 0; i < final_elements.size(); i++) {
        if(element.name == final_elements[i].name) {
            return true;
        }
    }
    return false;
}

bool are_final_elements(std::vector<Chemical> check_elements) {
    bool element_ok = false;
    for(int i = 0; i < check_elements.size(); i++) {
        if(!is_final_element(check_elements[i])) {
            return false;
        }
    }
    return true;
}

int index_in_vector(std::vector<Chemical> solution, Chemical element) {
    for(int i = 0; i < solution.size(); i++) {
        if(solution[i].name == element.name) {
            return i;
        }
    }
    return -1;
}

int get_reaction_quantity(std::string element_name) {
    for(std::unordered_map<std::string, std::vector<Chemical>, DataHasher>::iterator it = reactions.begin();
     it != reactions.end(); it++) {
        if(it->first == element_name) {
            return reactions_name_quantity[it->first];
        }
    }
    return 0;
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
            fuel_inputs = inputs;
        } else {
            if(inputs.size() == 1 && inputs[0].name == ORE) {
                final_elements.push_back(output);
                reactions[output.name] = inputs;
                reactions_name_quantity[output.name] = output.quantity;
            } else {
                reactions[output.name] = inputs;
                reactions_name_quantity[output.name] = output.quantity;
            }
        }
    }

    std::vector<Chemical> solution;
    for(int i = 0; i < fuel_inputs.size(); i++) {
        solution.push_back(fuel_inputs[i]);
    }

    // key : chemical name, velue: residue quantity
    std::unordered_map<std::string, int> residue_map; 

    for(std::unordered_map<std::string, std::vector<Chemical>, DataHasher>::iterator it = reactions.begin();
     it != reactions.end(); it++) {
        residue_map[it -> first] = 0;
    }

    int index = 0;
    int multiplier;
    int help;
    while(!are_final_elements(solution)) {
        Chemical element = solution[index];

        if(is_final_element(element)) {
            index++;
            continue;
        }

        int solution_index = index_in_vector(solution, element);
        solution.erase(solution.begin() + solution_index);
        std::vector<Chemical> inputs = reactions[element.name];
        multiplier = get_reaction_quantity(element.name);

        while(residue_map[element.name] > 0 && multiplier < element.quantity) {
            multiplier++;
            residue_map[element.name]--;
        }

        help = multiplier;
        // multiplier is lower than element.quantity
        if(multiplier < element.quantity) {
            // element.quantity is divided by multiplier 
            if(element.quantity % multiplier == 0) {
                multiplier = element.quantity / help;
            } else {
                // there is residue
                multiplier = element.quantity / help + 1;
            }
        } else {
            multiplier = 1;
        }
        residue_map[element.name] += (multiplier * help - element.quantity);

        for(int i = 0; i < inputs.size(); i++) {
            solution_index = index_in_vector(solution, inputs[i]);
            int mul = inputs[i].quantity * multiplier;
            if(solution_index == -1) {
                Chemical help_chemical(inputs[i].name, mul);
                solution.push_back(help_chemical);
            } else {
                solution[solution_index].quantity += mul;
            }
        }
    }

    long long int count = 0;

    for(int i = 0; i < solution.size(); i++) {
        Chemical element = solution[i];
        multiplier = get_reaction_quantity(element.name);

        if(multiplier < element.quantity) {
            help = multiplier;
            if(element.quantity % multiplier == 0) {
                multiplier = element.quantity / help;
            } else {
                multiplier = element.quantity / help + 1;
            }
        } else {
            multiplier = 1;
        }

        std::vector<Chemical> inputs = reactions[element.name];

        // inputs[0] = 0RE
        count += (inputs[0].quantity * multiplier);
    }

    std::cout << "part1 = " << count << std::endl;

    return 0;
}