#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

long long int part1(std::ifstream& file);
long long int part2(std::ifstream& file);

int main() {

    std::ifstream file("./day1.txt");

    std::cout << "Fuel for part 1 = " << part1(file) << std::endl;

    file.seekg(0, file.beg);

    std::cout << "Fuel for part 2 = " << part2(file) << std::endl;

}

long long int part1(std::ifstream& file) {
    std::string line;
    long long int total = 0;

    if(file.is_open()) {

        while (!file.eof())
        {
            getline(file, line);
            total +=  floor(std::stoi(line)/3)-2;
        }
    }

    return total;
}

long long int part2(std::ifstream& file) {
    std::string line;
    long long int total = 0;
    long long int tmp;

    if(file.is_open()) {

        while (!file.eof())
        {
            getline(file, line);
            tmp = floor(std::stoi(line)/3)-2;

            while(tmp > 0) {
                total += tmp;
                tmp = floor(tmp/3)-2;
            }
        }
    }

    return total;
}