#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

#define MIN 0

class Coordinate {
    public:
        Coordinate(int x, int y) {
            _x = x + 0.5;
            _y = y + 0.5;
        }
    
        double calculate_angle(int point_x, int point_y) {
            double x = point_x + 0.5;
            double y = point_y + 0.5;
            double help = (x-_x)/(y-_y); 
            return atan2((x-_x),(y-_y));
        }

        void set_max(int max) {
            _max = max;
        }

        int get_max() {
            return _max;
        }

        int get_x() {
            return _x;
        }

        int get_y() {
            return _y;
        }
    
    private:
        double _x; 
        double _y;
        int _max;
};

Coordinate findMaxCoordinate(int** matrix, int rows, int columns) 
{ 
    int maxElement = MIN; 
    int max_i;
    int max_j;

    for (int i = 0; i < rows; i++) { 
        for (int j = 0; j < columns; j++) { 
            if (matrix[i][j] > maxElement) { 
                maxElement = matrix[i][j];
                max_i = i;
                max_j = j;
            } 
        } 
    } 

    Coordinate coordinate(max_i, max_j);
    coordinate.set_max(maxElement);

    return coordinate; 
} 

int** build_matrix(std::vector<std::string> vector_array, int rows, int columns, 
                   std::vector<std::pair<int, int>>& pairs) 
{
    int** matrix = new int*[rows];
    int i = 0;
    int j = 0;
    std::for_each(vector_array.begin(), vector_array.end(), [&](std::string tmp) {
        matrix[i] = new int[columns];
        std::for_each(tmp.begin(), tmp.end(), [&](char c) {
            matrix[i][j] = c == '.' ? 0 : 1;   
            if(c == '#') {
                pairs.push_back(std::make_pair(i, j));
            }
            j++;
        });
        i++;
        j = 0;
    });
    
    return matrix;
}

Coordinate part1(int **matrix, int rows, int columns, std::vector<std::pair<int, int>>& pairs) {
    std::vector<double> angles;
    int count = 0;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            if(matrix[i][j] == 0) continue;
            angles.clear();
            Coordinate coordinate(i, j);
            for(int k = 0; k < pairs.size(); k++) {
                if(pairs[k].first == i && pairs[k].second == j) {
                    continue;
                }
                double angle = coordinate.calculate_angle(pairs[k].first, pairs[k].second);
                // if angle doesn't exist in vector of angles
                if(std::find(angles.begin(), angles.end(), angle) == angles.end()) {
                    angles.push_back(angle);
                    count++;
                }
            }
            matrix[i][j] = count;
            count = 0;
        }
    }

    return findMaxCoordinate(matrix, rows, columns);
}

int part2() {
    return 1;
}

int main()
{
    std::fstream file("./day10.txt");
    std::string str;
    std::vector<std::string> vector_array;
    int rows = 0;
    int columns = 0;
    
    while(getline(file, str)) {
        columns = str.size();
        rows++;
        vector_array.push_back(str);
    }
    
    std::vector<std::pair<int, int>> pairs;
    int** matrix = build_matrix(vector_array, rows, columns, pairs);
    
    Coordinate monitoring_station = part1(matrix, rows, columns, pairs);

    std::cout << "part1 = " << monitoring_station.get_max() << std::endl;
    std::cout << "part2 = " << part2() << std::endl;

    return 0;
}
