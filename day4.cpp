#include <iostream>
#include <string>
#include <math.h>

#define LOWER 248345
#define HIGHER 746315

/*
It is a six-digit number.
The value is within the range given in your puzzle input.
Two adjacent digits are the same (like 22 in 122345).
Going from left to right, the digits never decrease; they only ever increase or stay the same (like 111123 or 135679).
*/
int main() {

    int counter = 0;
    int tmp = 0;
    int previous = 0;

    for(int i = LOWER; i <= HIGHER; i++) {

        for(int j = 1; j <= 6; j++) {
            tmp = pow(10, i);
            if(previous < i/tmp) {

            }
            previous = i/tmp;
        }

    }
}