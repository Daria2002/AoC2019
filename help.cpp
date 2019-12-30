#include <stdio.h>
#include <iostream>
#include <vector>

int main() {
    std::vector<long long int> v;

    for(int i = 0; i < 2612; i++)
        v.push_back(1);

    for(int i = v.size(); i <= 5216; i++)
        v.push_back(1);

}