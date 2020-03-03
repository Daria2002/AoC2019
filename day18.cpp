#include <cstdio>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <cctype>
#include <algorithm>

class Position {
    public:
        Position() = default;
        Position(int _x, int _y) : x(_x), y(_y) {}
        int x, y;
};

bool operator==(const Position& position1, const Position& position2) {
    return (position1.x == position2.x && position1.y == position2.y);
}

class Base {
    public:
        Base(std::string _name, Position _position) : name(std::move(_name)), position(_position) {}
        std::string name;
        Position position;
};

bool operator==(const Base& x, const Base& y) {
    return x.name == y.name;
}

struct hashBase
{
    int operator()(const Base& base) const {
        return base.position.x-base.position.y;
    }
};

class Door : public Base {
    public:
        Door(std::string _name, Position _position) :
            Base(std::move(_name), _position) 
        {

        }

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
        Key(std::string _name, Position _position) : 
            Base(std::move(_name), _position) {}
};

class Field {
    public:
        explicit Field(const std::string& file_name) {
            std::vector<std::string> lines;
            std::string line;
            std::ifstream in(file_name);
            int row {0};
            while(std::getline(in, line)) {
                process_row(row, line);
                row++;
            }
        }

        bool is_reachable(const Position& position) {
            // check if rechable
            if(position.x > enterence_position.x && position.y == enterence_position.y) {
                return check_right(position, position.y);
            } else if(position.x < enterence_position.x && position.y == enterence_position.y) {
                return check_left(position, position.y);
            } else if(position.y > enterence_position.y && position.x == enterence_position.x) {
                return check_down(position, position.x);
            } else if(position.y < enterence_position.y && position.x == enterence_position.x) {
                return check_up(position, position.x);
            } else {
                // if enterence and element is not in the same row either column
                return check_if_there_is_a_path(position);
            }
        }

        inline void add_door(const Door& door) {
            doors.insert(door);
        }

        inline void add_key(const Key& key) {
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

        bool more_options(std::unordered_set<Door, hashBase>& available_doors, std::unordered_set<Key, hashBase>& available_keys) {
            available_doors = get_available_doors();
            available_keys = get_available_keys();
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
                std::string tmp(1, el);
                if(el == '.') {
                    path.push_back(position);
                } else if(el == '#') {
                    wall.push_back(position);
                } else if(isupper(el)) {
                    doors.insert(Door(tmp, position));
                } else {
                    keys.insert(Key(tmp, position));
                } 
                column++;
            });
        }

        template <typename T>
        bool check_if_element_to_process(const Position& position, std::unordered_set<T, hashBase> set) {
            for(int i = 0; i < set.size(); i++) {
                if(set[i].position == position) {
                    return true;
                }
            }
            return false;
        }

        template <typename T>
        bool contains(const Position& position, std::unordered_set<T, hashBase> set) const {
            const auto contains_position = [&](const auto& el) { return el.position == position; };
            return std::any_of(set.begin(), set.end(), contains_position);
        }

        bool contains(const Position& position, std::vector<Position> vector) const {
            const auto contains_position = [&](const auto& el) { return el == position; };
            return std::any_of(vector.begin(), vector.end(), contains_position);
        }

        template <typename T>
        bool has_obstacle(const Position& position, std::unordered_set<T, hashBase> set, std::vector<Position> vector) const {
            return (contains(position, set) || contains(position, std::move(vector)));
        }

        std::unordered_set<Door, hashBase> doors;
        std::unordered_set<Key, hashBase> keys;
        std::vector<Position> wall;
        std::vector<Position> path;

    private:   
        bool check_if_there_is_a_path(Position position) {
            // TODO
            return false;
        }

        bool check_right(const Position& position, int y) {
            // check that between enterence_position and position there are only .
            // or keys or doors that can be unlocked
            for(int i = enterence_position.x; i < position.x; i++) {
                if(has_obstacle(Position(i, y), doors, wall)) {
                    return false;
                }
            }
            return true;
        }

        bool check_left(const Position& position, int y) {
            for(int i = position.x + 1; i < enterence_position.x; i++) {
                if(has_obstacle(Position(i, y), doors, wall)) {
                    return false;
                }
            }
            return true;
        }

        bool check_down(const Position& position, int x) {
            for(int i = enterence_position.y + 1; i < position.y; i++) {
                if(has_obstacle(Position(x, i), doors, wall)) {
                    return false;
                } 
            }
            return true;
        }

        bool check_up(const Position& position, int x) {
            for(int i = position.y + 1; i < enterence_position.y; i++) {
                if(has_obstacle(Position(x, i), doors, wall)) {
                    return false;
                }
            }
            return true;
        }

        Position enterence_position;

        std::unordered_set<Door, hashBase> get_available_doors() {
            std::unordered_set<Door, hashBase> available_doors;
            std::for_each(doors.begin(), doors.end(), [&](auto const& el) {
                if(is_reachable(el.position)) {
                    available_doors.insert(el);
                }
            });
            return available_doors;
        }

        std::unordered_set<Key, hashBase> get_available_keys() {
            std::unordered_set<Key, hashBase> available_keys;
            std::for_each(keys.begin(), keys.end(), [&](auto const& el) {
                if(is_reachable(el.position)) {
                    available_keys.insert(el);
                }
            });
            return available_keys;
        }
};

template <typename T>
inline bool is_explored(std::unordered_set<T, hashBase> elements) {
    return elements.empty();
}

int get_shortest_path() {
    int counter = 0;
    const std::string file_name = "day18.txt";
    Field field(file_name);
    Position enterence_position = field.get_enterence_position();

    std::unordered_set<Door, hashBase> available_doors;
    std::unordered_set<Key, hashBase> available_keys;
    while(is_explored(field.doors) || is_explored(field.keys)) {
        if(field.more_options(available_doors, available_keys)) {
            // split in more cases

        } else {
            // go further, don't split... there is only one option to go, so go there
            if(!available_doors.empty()) {
                enterence_position = available_doors.begin() -> position;
            }
            else {
                enterence_position = available_keys.begin() -> position;
            }
        }
    }
    return counter;
}

int main() {
    int shortest_path = get_shortest_path();
    std::cout << "shortest path = " << shortest_path << std::endl;
    return 0;
}