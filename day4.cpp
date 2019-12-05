#include <iostream>
#include <string>
#include <math.h>

#define LOWER 248345
#define HIGHER 746315

#define ASCII_ZERO 48

int part1();

/*
It is a six-digit number.
The value is within the range given in your puzzle input.
Two adjacent digits are the same (like 22 in 122345).
Going from left to right, the digits never decrease; they only ever increase or stay the same (like 111123 or 135679).
*/
int main() {

    std::cout << "part1 = " << part1() << std::endl;
    

}

int part1() {
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
            if(last_digit == digit) {
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