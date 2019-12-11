#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>
/*
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace boost::algorithm;
*/
int part1(std::unordered_map<std::string, std::string> map);
int part2(std::unordered_map<std::string, std::string> map, std::string str,
 int count, std::string last);
std::vector<std::string> splitStringToArray(std::string str, char token);

int main() {
    std::ifstream file("./day6.txt");
    std::string str;

    std::unordered_map<std::string, std::string> map;
    std::vector<std::string> orbits;
   
    while(std::getline(file, str)) {
        orbits = splitStringToArray(str, ')');
        map[orbits[1]] = orbits[0];
    }
   
    std::cout << "part1 = " << part1(map) << std::endl;
    std::cout << "part2 = " << part2(map, "YOU", 0, "") << std::endl;
   
    return 0;
}

std::vector<std::string> splitStringToArray(std::string str, char token) {
    std::replace(str.begin(), str.end(), token, ' ');
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string tmp;
    while(ss >> tmp) {
        v.push_back(tmp);
    }
    return v;
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
    std::string second;
    auto it = map.begin();
    std::string first;
    while(it != map.end()) {
        it = map.find(str);
        if(str == "SAN") {
            return 1;
        }
        if(str == "COM") {
            return -1;
        }
        second = it -> second;
        std::cout << "second = " << second << std::endl;
        if(second != last)
            count = part2(map, second, count, str);


        for(auto el:map) {
            if(el.second != str)
                continue;
            first = el.first;
            part2(map, first, count, str);
        }

        it++;
    }
    return count++;
} 


/*
COM)B
B)C
C)D
D)E
E)F
B)G
G)H
D)I
E)J
J)K
K)L
K)YOU
I)SAN
*/