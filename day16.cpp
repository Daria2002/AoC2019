#include <stdio.h>
#include <ostream>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <algorithm>
#include <math.h>

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
            long long int index;
            index = j < base_pattern.size() - 1 ? j % new_base_pattern.size() + 1 : (j+1) % new_base_pattern.size();
            sum += input[j] * new_base_pattern[index];
        }
        int last_digit = abs(sum) % 10;
        output.push_back(last_digit);
    }

    return output;
}

std::vector<int> get_new_input_part2(long long int offset, std::vector<int> input, 
std::vector<int> base_pattern) {
    std::vector<int> output;
    std::vector<int> new_base_pattern;
    long long int sum = 0, compute_help = 0;
    for(long long int i = input.size() - 1; i >= offset; i--) {
        sum += input[i];
        output.push_back(sum % 10);
    }
    for(int i = offset-1; i >= 0; i--) {
        output.push_back(0);
    }
    std::reverse(output.begin(), output.end());
    return output;
}

long long int calculate_offset(std::vector<int> input_elements) {
    std::vector<int> offset_values(input_elements.begin(), input_elements.begin()+7);
    long long int offset = 0;
    for(int k = 0; k < 7; k++) {
        offset += offset_values[k] * pow(10, 7-k-1);
    }
    return offset;
}

int main() {
    std::vector<int> base_pattern = {0, 1, 0, -1};
    std::vector<int> input_elements = get_input_elements("./day16.txt");

    for(int i = 0; i < NUM_OF_ITER; i++) {
        input_elements = get_new_input(input_elements, base_pattern);
    }

    std::cout << "part1:" << std::endl;
    std::vector<int> result1(input_elements.begin(), input_elements.begin()+8);
    std::cout << result1 << std::endl;

    std::cout << "part2:" << std::endl;
    input_elements = get_input_elements("./day16.txt");
    std::vector<int> input_elements2;
    for(int i = 0; i < 10000; i++) {
        input_elements2.insert(input_elements2.end(), input_elements.begin(), input_elements.end());
    }
    long long int offset = calculate_offset(input_elements);
    for(int i = 0; i < NUM_OF_ITER; i++) {
        input_elements2 = get_new_input_part2(offset, input_elements2, base_pattern);
    }
    std::vector<int> result2(input_elements2.begin() + offset, input_elements2.begin() + offset + 8);
    std::cout << result2 << std::endl;    
}