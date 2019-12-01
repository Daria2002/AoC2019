#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

long long int part1(std::ifstream& file1);
long long int part2(std::ifstream& file2);

int main() {

    std::ifstream file1("./day1.txt");
    std::ifstream file2("./day1_part2.txt");

    std::cout << "Fuel for part 1 = " << part1(file1) << std::endl;
    std::cout << "Fuel for part 2 = " << part2(file2) << std::endl;

}

long long int part1(std::ifstream& file1) {
    std::string line;
    long long int total = 0;

    if(file1.is_open()) {

        while (!file1.eof())
        {
            getline(file1, line);
            total +=  floor(std::stoi(line)/3)-2;
        }
    }

    return total;
}

long long int part2(std::ifstream& file2) {
    std::string line;
    long long int total = 0;
    long long int tmp;

    if(file2.is_open()) {

        while (!file2.eof())
        {
            getline(file2, line);
            tmp = floor(std::stoi(line)/3)-2;

            while(tmp > 0) {
                total += tmp;
                tmp = floor(tmp/3)-2;
            }
        }
    }

    return total;
}