#include <sstream>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream file("day8.txt");
    std::string str;
    std::getline(file, str);
    std::stringstream ss(str);

    std::cout << str << std::endl;
}