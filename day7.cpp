#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <algorithm>

int main() {

    std::ifstream file("day7.txt");

    std::string str;
    std::getline(file, str);

    //  using int array
    // int phases[] = {0, 1, 2, 3, 4};
    // std::sort(phases, phases+5);

    std::array<int, 5> phase_numbers = {0, 1, 2, 3, 4};
    std::sort(phase_numbers.begin(), phase_numbers.end());
    
    do {

    } while (std::next_permutation(phase_numbers.begin(), phase_numbers.end()));
}
