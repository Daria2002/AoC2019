#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <ctype.h>
#include <algorithm>

class Position {
    public:
        Position() {}
        Position(int _x, int _y) : x(_x), y(_y) {}
        int x, y;
};

class Base {
    public:
        Base() {}
        Base(std::string _name, Position _position) : name(_name), position(_position) {}
        std::string name;
        Position position;
        bool is_reachable(const Position& enterence_position, const Field& field) {
            // check if rechable
            if(position.x > enterence_position.x && position.y == enterence_position.y) {
                return check_right(enterence_position, field, position.y);
            } else if(position.x < enterence_position.x && position.y == enterence_position.y) {
                return check_left();
            } else if(position.y > enterence_position.y && position.x == enterence_position.x) {
                return check_down();
            } else if(position.y < enterence_position.y && position.x == enterence_position.x) {
                return check_up();
            } else {
                
            }
        }
    private:
        bool check_right(const Position& enterence_position, const Field& field, int y) {
            // check that between enterence_position and position there are only .
            // or keys or doors that can be unlocked
            for(int i = enterence_position.x; i < position.x; i++) {
                // if position == unlocked door return false
                if(field.contains(Position(i, y), field.doors)) {
                    return false;
                }
            }
            return true;
        }


};

bool operator==(const Base& x, const Base& y) {
    return x.name == y.name;
}

struct hashBase
{
    std::string operator()(const Base& base) const {
        return base.name;
    }
};

class Door : public Base {
    public:
        Door() {}
        inline void unlock() {
            locked = false;
        }
        inline bool is_locked() const {
            return locked;
        }
    private:
        bool locked = true;
};

class Key : public Base {
    public: 
        Key() {}
};

class Field {
    public:
        Field() {}

        inline void add_door(Door door) {
            doors.insert(door);
        }

        inline void add_key(Key key) {
            keys.insert(key);
        }

        inline void open_door(Door door) {
            doors.erase(door);
            path.push_back(door.position);
        }
        
        inline void pick_up_key(Key key) {
            keys.erase(key);
            path.push_back(key.position);
        }

        inline int count_options(const std::unordered_set<Door, hashBase>& available_doors, 
        const std::unordered_set<Key, hashBase>& available_keys) {
            int counter = 0;
            counter = available_doors.size() + available_keys.size();
            return counter;
        }

        bool more_options(Position enterence_position) {
            std::unordered_set<Door, hashBase> available_doors = get_available_doors();
            std::unordered_set<Key, hashBase> available_keys = get_available_keys();
            return (count_options(available_doors, available_keys) > 1);
        }

        Position get_enterence_position() {
            return enterence_position;
        }

        void set_enterence_position(Position position) {
            enterence_position = position;
        }

        void process_row(const int& row_number, const std::string& row) {
            int column = 0;
            std::for_each(row.begin(), row.end(), [&] (auto const& el) {
                Position position(row_number, column);
                if(el == '.') path.push_back(position);
                else if(el == '#') wall.push_back(position);
                else if(isupper(el)) doors.insert(Door(el, position));
                else keys.insert(Key(el, Position));
                column++;
            });
        }

        template <typename T>
        bool check_if_element_to_process(const Position& position, std::unordered_set<T, hashBase> set) {
            for(int i = 0; i < set.size(); i++) {
                if(set[i].position == position) return true;
            }
            return false;
        }

        template <typename T>
        bool contains(const Position& position, std::unordered_set<T, hashBase> set) const {
            std::for_each(set.begin(), set.end(), [&](const auto& el) {
                if(el.position == position) return true;
            })
            return false;
        }

        std::unordered_set<Door, hashBase> doors;
        std::unordered_set<Key, hashBase> keys;
        std::vector<Position> wall;
        std::vector<Position> path;

    private:   
        Position enterence_position;

        std::unordered_set<Door, hashBase> get_available_doors() {
            std::unordered_set<Door, hashBase> available_doors;
            std::for_each(doors.begin(), doors.end(), [&](auto const& el) {
                if(!el.is_locked() && el.is_reachable(enterence_position)) {
                    available_doors.insert(el);
                }
            });
            return available_doors;
        }

        std::unordered_set<Key, hashBase> get_available_keys() {
            std::unordered_set<Key, hashBase> available_keys;
            std::for_each(keys.begin(), keys.end(), [&](auto const& el) {
                if(el.is_reachable(enterence_position)) {
                    available_keys.insert(el);
                }
            });
            return available_keys;
        }
};

template <typename T>
inline bool is_explored(std::unordered_set<T, hashBase> elements) {
    return elements.size() > 0;
}

Field get_field() {
    Field field;
    std::vector<std::string> lines;
    std::string line;
    std::ifstream in("day18.txt");
    int row {0};
    while(std::getline(in, line)) {
        field.process_row(row, line);
        row++;
    }
    return field;
}

int get_shortest_path() {
    int counter = 0;
    Field field = get_field();
    Position enterence_position = field.get_enterence_position();

    while(is_explored(field.doors) || is_explored(field.keys)) {
        if(field.more_options(enterence_position)) {
            // split in more cases
        } else {
            // go further, don't split
        }
    }
    return counter;
}

int main() {
    int shortest_path = get_shortest_path();
    std::cout << "shortest path = " << shortest_path << std::endl;
}