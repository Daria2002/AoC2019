#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <map>

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

        void set_angles(std::vector<double> angles) {
            _angles = angles;
        }

        std::vector<double> get_angles() {
            return _angles;
        }
    
    private:
        double _x; 
        double _y;
        int _max;
        std::vector<double> _angles;
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

Coordinate part1(int **matrix, int rows, int columns, std::vector<std::pair<int, int>>& asteroids) {
    int** scores = new int*[rows];
    std::vector<double> angles;
    int count = 0;
    for(int i = 0; i < rows; i++) {
        scores[i] = new int[columns];
        for(int j = 0; j < columns; j++) {
            if(matrix[i][j] == 0) {
                scores[i][j] = 0;
                continue;
            } 
            angles.clear();
            Coordinate coordinate(i, j);
            for(int k = 0; k < asteroids.size(); k++) {
                if(asteroids[k].first == i && asteroids[k].second == j) {
                    continue;
                }
                double angle = coordinate.calculate_angle(asteroids[k].first, asteroids[k].second);
                // if angle doesn't exist in vector of angles
                if(std::find(angles.begin(), angles.end(), angle) == angles.end()) {
                    angles.push_back(angle);
                    count++;
                }
            }
            scores[i][j] = count;
            count = 0;
        }
    }

    return findMaxCoordinate(scores, rows, columns);
}

int part2(Coordinate monitoring_station, int** matrix, int rows, int columns, std::vector<std::pair<int, int>>& asteroids, 
          std::map<double, std::vector<std::pair<int, int>>> map_angle_and_asteroids) {
    std::vector<double> angles;
    int count = 0;
    int j, i;
    int not_finished = true;
    for(i = 0; i < rows && not_finished == true; i++) {
        not_finished = false;
        for(j = 0; j < columns; j++) {
            if(matrix[i][j] == 0) continue;

            angles.clear();
            Coordinate coordinate(i, j);
            for(int k = 0; k < asteroids.size(); k++) {
                if(asteroids[k].first == i && asteroids[k].second == j) {
                    continue;
                }
                double angle = coordinate.calculate_angle(asteroids[k].first, asteroids[k].second);
                // if angle doesn't exist in vector of angles
                if(std::find(angles.begin(), angles.end(), angle) == angles.end()) {
                    angles.push_back(angle);
                    count++;
                    if(count == 200) {
                        return (asteroids[k].second) * 100 + (asteroids[k].first);
                    }
                    // vaporization
                    matrix[i][j] = 0;
                } else {
                    // there is asteroid but it cannot be detected
                    not_finished = true;
                }
            }
        }
    }

    return 1;
}

std::map<double, std::vector<std::pair<int, int>>> build_asteroids_map(Coordinate monitoring_station, int** matrix, int rows, int columns) {
    std::map<double, std::vector<std::pair<int, int>>> map;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            double angle = monitoring_station.calculate_angle(i, j);
            std::vector<std::pair<int, int>> v;
            // add angle if it doesn't exist, otherwise append vector of pairs
            if(map.find(angle) != map.end) {
                v = map[angle];
                v.push_back(std::make_pair(i, j));
                map[angle] = v;
            } else {
                v.push_back(std::make_pair(i, j));
                map[angle] = v;
            }
        }
    }
    return map;
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
    
    // only asteroids
    std::vector<std::pair<int, int>> asteroids;
    int** matrix = build_matrix(vector_array, rows, columns, asteroids);
    std::vector<Coordinate> coordinates;

    // key = angle
    // value = vector of pairs with angle = key
    std::map<double, std::vector<std::pair<int, int>>> map_angle_and_asteroids;

    Coordinate monitoring_station = part1(matrix, rows, columns, asteroids);

    std::cout << "part1 = " << monitoring_station.get_max() << std::endl;

    map_angle_and_asteroids = build_asteroids_map(monitoring_station, matrix, rows, columns);

    std::cout << "part2 = " << part2(monitoring_station, matrix, rows, columns, asteroids, map_angle_and_asteroids) << std::endl;

    return 0;
}
