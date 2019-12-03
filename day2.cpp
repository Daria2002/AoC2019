#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include <streambuf>
#include <functional>
#include <map>

#define HALT 99
#define ADD 1
#define MULTIPLY 2

const std::vector<int> explode(const std::string& s, const char& c)
{
	std::string buff{""};
	std::vector<int> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(std::stoi(buff)); buff = ""; }
	}
	if(buff != "") v.push_back(std::stoi(buff));
	
	return v;
}

void calculateNumbers(std::vector<int>& numbers, std::map<int, std::function<int(int, int)>>& functions)
{
	for(int i = 0; i < numbers.size(); i = i+4) {
        if(numbers[i] == 99) {
            break;
        }
        numbers[numbers[i+3]] = functions[numbers[i]](numbers[numbers[i+1]], numbers[numbers[i+2]]);
    }
}

int main() {

    std::ifstream file("./day2.txt");
    
    std::string temp;
    std::getline(file, temp);

    std::vector<int> numbers{explode(temp, ',')};

    std::map<int, std::function<int(int, int)>> functions;
    
    functions.emplace(ADD, [](int x, int y)->int{return x+y;});
    functions.emplace(MULTIPLY, [](int x, int y)->int{return x*y;});

    numbers[1] = 12;
    numbers[2] = 2;

    calculateNumbers(numbers, functions);

    std::cout << numbers[0] << std::endl;
 
    for(int i = 1; i <= 99; i++) {
        for(int j = 1; j <= 99; j++) {
            numbers = explode(temp, ',');
            numbers[1] = i;
            numbers[2] = j;

            calculateNumbers(numbers, functions);

            if(numbers[0] == 19690720) {
                std::cout << 100 * i + j << std::endl;
                break;
            }
        }    
    }

    return 0;
}

