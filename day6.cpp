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
 std::string last);
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
    std::cout << "part2 = " << part2(map, map.find("YOU")->second, "YOU")-1 << std::endl;
   
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
std::string last) {
    int count = map.size();
    if(str == "SAN") {
        return 0;
    } 
    if(str == "COM") {
        return map.size();
    }

    auto it = map.find(str);
    if(it != map.end() && it->second != last) {
        count = std::min(count, 1 + part2(map, it->second, str));
    }   

    it = map.begin();
    while(1) {
        it = std::find_if(std::next(it), map.end(), [&](std::pair<std::string, std::string> p) 
            {return (p.second == str);});
        if(it == map.end()) break; 
        count = std::min(count, 1 + part2(map, it->first, str));
    }

    return count;
}