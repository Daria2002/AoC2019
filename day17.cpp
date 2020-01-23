#include <stdio.h>
#include <vector>
#include <ostream>
#include <iostream>

template<class T>
std::ostream& operator<<(std::ostream& stream, const std::pair<T, T>& pair) {
    stream << "(" << pair.first << ", " << pair.second << ")" << std::endl;
    return stream;
}

template<class T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& v) {
    stream << "[";
    for(int i = 0; i < v.size(); i++) {
        if(i != 0) stream << ", ";
        stream << v[i];
    }
    stream << "]";
    return stream;
} 

int main() {
    
}