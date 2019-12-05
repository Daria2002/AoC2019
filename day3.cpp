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
        functions.emplace(LEFT, [&](int val, std::vector<std::pair<int, int>>& pairs_tmp){
            for(int i = 0; i < val; i++) {
                current_x -= 1;
                std::pair<int, int> tmp_pair = std::make_pair(current_x, current_y);
                pairs_tmp.push_back(tmp_pair);
                steps_map[tmp_pair] = steps++;
            }});
        functions.emplace(RIGHT, [&](int val, std::vector<std::pair<int, int>>& pairs_tmp){
            for(int i = 0; i < val; i++) {
                current_x += 1;
                std::pair<int, int> tmp_pair = std::make_pair(current_x, current_y);
                pairs_tmp.push_back(tmp_pair);
                steps_map[tmp_pair] = steps++;
            }});
        functions.emplace(UP, [&](int val, std::vector<std::pair<int, int>>& pairs_tmp){
            for(int i = 0; i < val; i++) {
                current_y += 1;
                std::pair<int, int> tmp_pair = std::make_pair(current_x, current_y);
                pairs_tmp.push_back(tmp_pair);
                steps_map[tmp_pair] = steps++;
            }});
        functions.emplace(DOWN, [&](int val, std::vector<std::pair<int, int>>& pairs_tmp){
            for(int i = 0; i < val; i++) {
                current_y -= 1;
                std::pair<int, int> tmp_pair = std::make_pair(current_x, current_y);
                pairs_tmp.push_back(tmp_pair);
                steps_map[tmp_pair] = steps++;
            }});
        pairs = make_pairs(split_line(line));
    }

    std::vector<std::pair<int, int>> get_pairs() {
        return pairs;
    }

    std::map<std::pair<int, int>, int> get_steps() {
        return steps_map;
    }

private:
    std::string line;
    std::vector<std::pair<int, int>> pairs;
    int current_x = 0;
    int current_y = 0;
    int steps = 1;
    std::map<std::pair<int, int>, int> steps_map;
    std::map<char, std::function<void(int, std::vector<std::pair<int, int>>&)>> functions;

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
            functions[instructions[i].at(0)](std::stoi(instructions[i].substr(1, instructions[i].size()-1)), pairs_tmp);
        }

        return pairs_tmp;
    }
};

int get_min(std::vector<std::pair<int, int>> first, std::vector<std::pair<int, int>> second, std::map<std::pair<int, int>, int>& intersections, 
std::map<std::pair<int, int>, int> steps1, std::map<std::pair<int, int>, int> steps2) {
    int min = pow(10, 5);

    for(int i = 0; i < first.size(); i++) {
        if(std::find(second.begin(), second.end(), first[i]) != second.end()) {
            intersections[first[i]] = steps1[first[i]] + steps2[first[i]];
            if((std::abs(first[i].first) + std::abs(first[i].second)) > 0 && (std::abs(first[i].first) + std::abs(first[i].second)) < min) {
                min = std::abs(first[i].first) + std::abs(first[i].second);
            }
        }
    }

    return min;
}

int get_min_steps( std::map<std::pair<int, int>, int> intersections) {
    int min = pow(10, 5);

    for(std::map<std::pair<int, int>,int>::iterator iter = intersections.begin(); iter != intersections.end(); ++iter)
    {
        int v = iter->second;
        if(v < min) {
            min = v;
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
    
    std::map<std::pair<int, int>, int> steps_map1 = l1.get_steps();
    std::map<std::pair<int, int>, int> steps_map2 = l2.get_steps();

    std::map<std::pair<int, int>, int> intersections;

    int min = get_min(pairs1, pairs2, intersections, steps_map1, steps_map2);
    std::cout << min << std::endl;

    min = get_min_steps(intersections);
    std::cout << min << std::endl;

    return 0;
}