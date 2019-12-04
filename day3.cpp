#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h> 
#include <functional>
#include <algorithm>
#include <math.h>
#include<bits/stdc++.h> 

#define LEFT 'L'
#define RIGHT 'R'
#define UP 'U'
#define DOWN 'D'

class Line 
{
public:
    Line(std::string str) {
        line = str;
        functions.emplace(LEFT, [&](int val, std::vector<std::pair<int, int>> pairs_tmp){
            for(int i = 0; i < val; i++) {
                current_x -= 1;
                pairs_tmp.push_back(std::make_pair(current_x, current_y));
            }
            return pairs_tmp;
            });
        functions.emplace(RIGHT, [&](int val, std::vector<std::pair<int, int>> pairs_tmp){
            for(int i = 0; i < val; i++) {
                current_x += 1;
                pairs_tmp.push_back(std::make_pair(current_x, current_y));
            }
            return pairs_tmp;
            });
        functions.emplace(UP, [&](int val, std::vector<std::pair<int, int>> pairs_tmp){
            for(int i = 0; i < val; i++) {
                current_y += 1;
                pairs_tmp.push_back(std::make_pair(current_x, current_y));
            }
            return pairs_tmp;
            });
        functions.emplace(DOWN, [&](int val, std::vector<std::pair<int, int>> pairs_tmp){
            for(int i = 0; i < val; i++) {
                current_y -= 1;
                pairs_tmp.push_back(std::make_pair(current_x, current_y));
            }
            return pairs_tmp;
            });
        pairs = make_pairs(split_line(line));
    }

    std::vector<std::pair<int, int>> get_pairs() {
        return pairs;
    }

private:
    std::string line;
    std::vector<std::pair<int, int>> pairs;
    int current_x = 0;
    int current_y = 0;
    std::map<char, std::function<std::vector<std::pair<int, int>>(int, std::vector<std::pair<int, int>>)>> functions;

    std::vector<std::string> split_line(std::string str) {
        std::stringstream ss(str);
        std::vector<std::string> v;

        while(getline(ss, str, ',')) {
            v.push_back(str);
        }

        return v;
    }

    std::vector<std::pair<int, int>> make_pairs(std::vector<std::string> instructions) {
        std::vector<std::pair<int, int>> pairs_tmp;

        for(int i = 0; i < instructions.size(); i++) {
            pairs_tmp = functions[instructions[i].at(0)](std::stoi(instructions[i].substr(1, instructions[i].size()-1)), pairs_tmp);
        }

        return pairs_tmp;
    }
};

int get_min(std::vector<std::pair<int, int>> first, std::vector<std::pair<int, int>> second) {
    int min = pow(10, 5);

    for(int i = 0; i < first.size(); i++) {
        if(std::find(second.begin(), second.end(), first[i]) != second.end()) {
            if((std::abs(first[i].first) + std::abs(first[i].second)) > 0 && (std::abs(first[i].first) + std::abs(first[i].second)) < min) {
                min = std::abs(first[i].first) + std::abs(first[i].second);
                break;
            }
        }
    }

    return min;
}

int main() {
    std::ifstream file("./day3.txt");

    std::string tmp;

    getline(file, tmp);
    Line l1(tmp);

    getline(file, tmp);
    Line l2(tmp);

    std::vector<std::pair<int, int>> pairs1 = l1.get_pairs();
    std::vector<std::pair<int, int>> pairs2 = l2.get_pairs();

    sort(pairs1.begin(), pairs1.end());
    sort(pairs2.begin(), pairs2.end());

    int min = pairs1.size() > pairs2.size() ? get_min(pairs1, pairs2) : get_min(pairs2, pairs1);
    std::cout << min << std::endl;
    
    return 0;
}