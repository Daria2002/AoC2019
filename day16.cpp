#include <stdio.h>
#include <ostream>
#include <vector>
#include <iostream>

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

int main() {
    
}