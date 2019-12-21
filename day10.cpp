#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <map>

#define MIN 0
#define TOL 1e-15

class Asteroid {
    public:
        double _x; 
        double _y;
        int _max;
        std::vector<double> _angles;

        Asteroid(int x, int y) {
            _x = x + 0.5;
            _y = y + 0.5;
        }

        double calculate_euclidian(int point_x, int point_y) {
            double x = point_x + 0.5;
            double y = point_y + 0.5;
            return sqrt(pow(x-_x, 2) + pow(y-_y, 2));
        }
    
        double calculate_angle(int point_x, int point_y) {
            double x = point_x + 0.5;
            double y = point_y + 0.5;
            return atan2((_y-y), -(_x-x));
        }

        void set_max(int max) {
            _max = max;
        }

        void set_angles(std::vector<double> angles) {
            _angles = angles;
        }
};

Asteroid findMonitoringStation(int** matrix, int rows, int columns) 
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
    
    // ovdje je max_j, max_i
    Asteroid coordinate(max_j, max_i);
    coordinate.set_max(maxElement);

    return coordinate; 
} 

int** build_matrix(std::vector<std::string> file_lines, int rows, int columns, 
                   std::vector<std::pair<int, int>>& pairs) 
{
    int** matrix = new int*[rows];
    int i = 0;
    int j = 0;
    std::for_each(file_lines.begin(), file_lines.end(), [&](std::string tmp) {
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

Asteroid part1(int **matrix, int rows, int columns, std::vector<std::pair<int, int>>& asteroids) {
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
            Asteroid coordinate(i, j);
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
    return findMonitoringStation(scores, rows, columns);
}

int part2(Asteroid monitoring_station, int** matrix, int rows, int columns, std::vector<std::pair<int, int>>& asteroids, 
          std::map<double, std::vector<std::pair<int, int>>> map_angle_and_asteroids) {

    for (auto& item : map_angle_and_asteroids)
        std::sort(item.second.begin(), item.second.end(),
            [&](std::pair<int, int>& a, std::pair<int, int>& b){
                return (monitoring_station.calculate_euclidian(a.first, a.second) < monitoring_station.calculate_euclidian(b.first, b.second));
            });

    // get element at 90 degrees
    auto iter = std::find_if(map_angle_and_asteroids.rbegin(), map_angle_and_asteroids.rend(), [&](auto& item) {
        if(item.first >= M_PI_2 && item.first < M_PI_2 + TOL) {
            return true;
        }
        return false;
    });
    const auto iter_start(iter);
    int count = 0;
    while (map_angle_and_asteroids.size() > 0)
    {
        if (iter->second.size() > 0) 
        {   
            count++;
            if(count == 200) {
                return iter->second[0].first * 100 + iter->second[0].second;
            }
            iter->second.erase(iter->second.begin()); // TODO: Change vector to list :)
            
        }
        iter++;
        if (iter == map_angle_and_asteroids.rend())
            iter = map_angle_and_asteroids.rbegin();
    }
    return -1;
}

std::map<double, std::vector<std::pair<int, int>>> build_angle_asteroids_map(Asteroid monitoring_station, int** matrix, int rows, int columns) {
    std::map<double, std::vector<std::pair<int, int>>> map;
    int c = 0;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            if(matrix[i][j] == 0 || (j == monitoring_station._x-0.5 && i == monitoring_station._y-0.5)) {
                continue;
            }
            c++;
            double angle = monitoring_station.calculate_angle(j, i);
            if(angle > 1.5 && angle < 1.7) {
                angle = monitoring_station.calculate_angle(j, i);
            }
            // std::cout << "za x = " << j << ", y = " << i << std::endl; 
            // std::cout << "calculated angle = " << angle << std::endl;
            std::vector<std::pair<int, int>> v;
            // add angle if it doesn't exist, otherwise append vector of pairs
            if(map.find(angle) != map.end()) {
                v = map[angle];
                v.push_back(std::make_pair(j, i));
                map[angle] = v;
            } else {
                v.push_back(std::make_pair(j, i));
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
    std::vector<Asteroid> coordinates;

    // key = angle
    // value = vector of pairs with angle = key
    std::map<double, std::vector<std::pair<int, int>>> map_angle_and_asteroids;

    Asteroid monitoring_station = part1(matrix, rows, columns, asteroids);
    std::cout << "part1 = " << monitoring_station._max << std::endl;

    map_angle_and_asteroids = build_angle_asteroids_map(monitoring_station, matrix, rows, columns);

    int part2_result = part2(monitoring_station, matrix, rows, columns, asteroids, map_angle_and_asteroids);
    std::cout << "part2 = " << part2_result << std::endl;

    return 0;
}
