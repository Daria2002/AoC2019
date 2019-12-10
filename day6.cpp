#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <array>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace boost::algorithm;

int part1(std::unordered_map<std::string, std::string> map);
int part2(std::unordered_map<std::string, std::string> map, std::string str,
 int count, std::string last);

int main() {
    std::ifstream file("./day6.txt");
    std::string str;

    std::unordered_map<std::string, std::string> map;
    std::vector<std::string> orbits;

    while(std::getline(file, str)) {
        split(orbits, str, is_any_of(")"));
        map[orbits[1]] = orbits[0];
    }
    
    std::cout << "part1 = " << part1(map) << std::endl;
    std::cout << "part2 = " << part2(map, "YOU", 0, "") << std::endl; 
    
    return 0;
}

int part1(std::unordered_map<std::string, std::string> map) {
    int count = 0;
    std::string tmp;

    for (auto el : map) {
        tmp = el.first;
        while(map.find(tmp) != map.end()) {
            count++;
            tmp = map[tmp];
        }
    }
    return count;
}

int part2(std::unordered_map<std::string, std::string> map, std::string str,
int count, std::string last) {
    // if destination reached
    if(str == "SAN") {
        return 1;
    }

    auto it = map.begin();
    std::string first;
    while(it != map.end()) {
        first = it -> first;
        count = part2(map, first, count, str);
        it++;
    }
    std::string second;
    for(auto el:map) {
        if(el.first != str)
            continue;
        second = el.second;
        if(second == str && second != last) {
            part2(map, second, count, str);
        }
    }
    return count++;
} 