#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <array>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace boost::algorithm;

int main() {
    std::ifstream file("./day6.txt");
    std::string str;

    std::unordered_map<std::string, std::string> map;
    std::vector<std::string> orbits;

    while(std::getline(file, str)) {
        split(orbits, str, is_any_of(")"));
        map[orbits[1]] = orbits[0];
    }

    int count = 0;
    std::string tmp;

    for (const auto &el : map) {
        tmp = el.first;

        while(map.find(tmp) != map.end()) {
            count++;
            tmp = map[tmp];
        }

    }

    std::cout << "count "<< count << std::endl;
    return 0;
}