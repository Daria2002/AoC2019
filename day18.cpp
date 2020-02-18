#include <stdio.h>
#include <iostream>
#include <vector>

class Door {

};

class Key {

};

class Field {
    public:
        std::vector<Door> doors;
        std::vector<Key> keys;
};

bool door_explored(std::vector<Door> doors) {
    return doors.size() > 0;
}

Field get_field() {
    Field field;
    // open file
    // read line by line and save in field
    return field;
}

int get_shortest_path() {
    int counter = 0;

    Field field = get_field();

    while(!door_explored(field.doors)) {
        
    }

    return counter;
}

int main() {
    int shortest_path = get_shortest_path();
    std::cout << "shortest path = " << shortest_path << std::endl;
}