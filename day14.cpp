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
  std::size_t operator () (const Chemical &key) const 
  {
    std::size_t seed = 0;
    boost::hash_combine(seed, boost::hash_value(key.name));
    boost::hash_combine(seed, boost::hash_value(key.quantity));
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
typedef std::unordered_map<Chemical, std::vector<Chemical>, DataHasher> DataHashMap;
DataHashMap reactions;
DataHashMap final_elements;
std::vector<Chemical> fuel_inputs;
typedef std::unordered_map<Chemical, int, DataHasher> OccurrencesMap;
OccurrencesMap final_elements_occurrences_map;

int count_occurrence(Chemical up, Chemical down, int counter) {
    return 1;
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
                final_elements[output] = inputs;
            } else {
                reactions[output] = inputs;
            }
        }
    }
    
    for(std::unordered_map<Chemical, std::vector<Chemical>, DataHasher>::iterator iter = final_elements.begin(); iter != final_elements.end(); ++iter) {
        for(int j = 0; j < fuel_inputs.size(); j++) {
            final_elements_occurrences_map[iter -> first] += count_occurrence(fuel_inputs[j], iter->first, 0);
        }
    }
    
    return 0;
}