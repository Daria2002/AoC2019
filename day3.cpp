#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h> 


std::vector<std::string> split_string(char * str) {
    char* p;
    p = strtok(str, ",");
    std::vector<std::string> v;

    // This end of the token is automatically
    // replaced by a null-character, and the
    // beginning of the token is returned by the function.
    while(p != NULL) {
        v.push_back(p);
        std::cout<<p<< std::endl;
        p = strtok(NULL, ",");
    }

}

int main() {
    std::ifstream file("./day3.txt");

    std::string first_line;
    std::string second_line;
    std::pair<int, int> first_coordinates;
    std::pair<int, int> second_coordinates;

    std::getline(file, first_line);
    std::getline(file, second_line);

    char char_arr[first_line.length()+1];
    strcpy(char_arr, first_line.c_str());
    split_string(char_arr);
    

}