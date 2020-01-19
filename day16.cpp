#include <stdio.h>
#include <ostream>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <algorithm>

#define NUM_OF_ITER 100

template<class T>
std::ostream& operator<<(std::ostream& stream, const std::pair<T, T>& pair) {
    stream << "(" << pair.first << ", " << pair.second << ")";
    return stream; 
}

template<class T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& values) {
  stream << "[ ";
  int count = 0;
  for (const auto& element : values) {
    if(count == 0) {
        count++;
    } else {
        std::cout << ", ";
    }
    std::cout << element;
  }
  stream << " ]";
  return stream;
}

std::vector<int> get_input_elements(std::string file_name) {
    std::ifstream file(file_name);
    std::string file_content;
    getline(file, file_content);
    std::vector<int> elements;

    std::for_each(file_content.begin(), file_content.end(),
     [&elements](const auto& c) {
         elements.push_back(c-'0');
    });

    return elements;
}

std::vector<int> adapt_base(std::vector<int> base_pattern, int num_of_replication) {
    std::vector<int> new_base;
    for(int k = 0; k < base_pattern.size(); k++) {
        for(int j = 0; j < num_of_replication+1; j++) {
            new_base.push_back(base_pattern[k]);
        }
    }
    return new_base;
}

std::vector<int> get_new_input(std::vector<int> input, std::vector<int> base_pattern) {
    std::vector<int> output;
    std::vector<int> new_base_pattern;
    for(int i = 0; i < input.size(); i++) {
        long long int sum = 0;
        new_base_pattern = adapt_base(base_pattern, i);
        
        for(int j = 0; j < input.size(); j++) {
            long long int index = j == 0 ? 1 : j % new_base_pattern.size() + 1;
            sum += input[j] * new_base_pattern[index];
        }
        int last_digit = abs(sum) % 10;
        output.push_back(last_digit);
    }

    return output;
}

int main() {
    std::vector<int> base_pattern = {0, 1, 0, -1};
    std::vector<int> input_elements = get_input_elements("./day16.txt");

    for(int i = 0; i < NUM_OF_ITER; i++) {
        input_elements = get_new_input(input_elements, base_pattern);
        std::cout << input_elements << std::endl;
    }

    std::cout << input_elements << std::endl;
}