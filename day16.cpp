#include <stdio.h>
#include <ostream>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <algorithm>

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

int main() {
    std::vector<int> base_pattern = {0, 1, 0, -1};

    std::vector<int> input_elements = get_input_elements("./day16.txt");

    std::cout << input_elements;
}