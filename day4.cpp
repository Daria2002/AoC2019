#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <functional>

#define LOWER 248345
#define HIGHER 746315

#define ASCII_ZERO 48

#define PART1 1
#define PART2 2

int part(int, std::map<int, std::function<bool(std::string, int, int)>>);

/*
It is a six-digit number.
The value is within the range given in your puzzle input.
Two adjacent digits are the same (like 22 in 122345).
Going from left to right, the digits never decrease; they only ever increase or stay the same (like 111123 or 135679).
*/
int main() {

    std::map<int, std::function<bool(std::string, int, int)>> functions;
    functions.emplace(PART1, [=](std::string num, int j, int digit) { return true;});

    functions.emplace(PART2, [=](std::string num, int j, int digit) {
        if((j == 1 || (j > 1 && num[j-2]-ASCII_ZERO != digit)) && (j == 5 || (j < 5 && (num[j+1]-ASCII_ZERO != digit)))) {
            return true;}});

    std::cout << "part1 = " << part(1, functions) << std::endl;
    std::cout << "part2 = " << part(2, functions) << std::endl;
}

int part(int part_number, std::map<int, std::function<bool(std::string, int, int)>> functions) {
    int counter = 0;
    int tmp = 0;
    int previous = 0;
    std::string num;
    int digit;
    int last_digit;
    bool ascending;
    bool duplicates_exists;

    for(int i = LOWER; i <= HIGHER; i++) {
        ascending = true;
        duplicates_exists = false;
        num = std::to_string(i);
        last_digit = num[0] - ASCII_ZERO;

        // check that digits never decrease
        for(int j = 1; j < 6; j++, ascending == true) {
            digit = num[j] - ASCII_ZERO;
            if(digit < last_digit) {
                ascending = false;
                break;
            }
            if(last_digit == digit && functions[part_number](num, j, digit) == true) {
                duplicates_exists = true;
            }
            last_digit = digit;
        }
        if(ascending == true && duplicates_exists == true) {
            counter++;
        }
    }

    return counter;
}