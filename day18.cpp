#include <cstdio>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <iostream>       
#include <string>         
#include <locale>
#include <cctype>
#include <unordered_map>

// (x, y)
class Position {
    public:
        Position() = default;
        Position(int _x, int _y) : x(_x), y(_y) {}
        int x, y;
        Position get_modified_position(int x_offset, int y_offset) const {
            return Position(x + x_offset, y + y_offset);
        }
};

class MyHashFunction { 
    public: 
        // Use sum of lengths of first and last names 
        // as hash function. 
        size_t operator()(const Position& p) const
        { 
            return p.x + p.y; 
        } 
};

std::ostream& operator<<(std::ostream& os, const Position& position)
{
    os << "(" << position.y << ", " << position.x << ")";
    return os;
}

bool operator==(const Position& position1, const Position& position2) {
    return (position1.x == position2.x && position1.y == position2.y);
}

bool operator!=(const Position& position1, const Position& position2) {
    return !(position1 == position2);
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
            Base(std::move(_name), _position) {
                locked = true;
            }

        inline void unlock() const {
            locked = false;
        }

        inline bool is_locked() const {
            return locked;
        }

    private:
        mutable bool locked;
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

        int check_if_there_is_a_path(Field field, const Position& position, Position imaginary_enterence,
                                const Position& former_position);

        bool is_reachable(const Position& position) { 
            if(position.x > enterence_position.x && position.y == enterence_position.y) { // if in same row, dest on right
                return check_right(position, position.y);
            }
            if(position.x < enterence_position.x && position.y == enterence_position.y) { // if in same row but dest is on the left
                return check_left(position, position.y);
            }
            if(position.y > enterence_position.y && position.x == enterence_position.x) {
                return check_down(position, position.x);
            } 
            if(position.y < enterence_position.y && position.x == enterence_position.x) {
                return check_up(position, position.x);
            }
            // if enterence and element is not in the same row either column
            return check_if_there_is_a_path(*this, position, enterence_position, enterence_position);
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
        
        bool unlock_door(const std::string& door_name) {
            for(const Door& door : doors) {
                if(door.name == door_name) {
                    door.unlock();
                    return true;
                }
            }
            return false;
        }

        std::string str_to_upper(const std::string& str) {
            std::string result;
            for(const auto el:str) {
                result += ((int)el - 97 + 65);
            }

            return result;
        }

        void enter_the_door(const Position& position) {
            for(Door door : doors) {
                if(door.position == position) {
                    doors.erase(door);
                    path.push_back(door.position);
                    return;
                }
            }
        }

        void pick_up_key(const Position& position) {
            std::string name;
            for(Key key:keys) {
                if(key.position == position) {
                    keys.erase(key);
                    name = key.name;
                    path.push_back(key.position);
                    break;
                }
            }
            std::string door_name = str_to_upper(name);
            if(!unlock_door(door_name)) {
                std::cout << "!! door " << door_name << " can't be unlocked." << std::endl;
            }
        }

        void pick_up_key(const Key& key) {
            keys.erase(key);
            path.push_back(key.position);
            std::string door_name = str_to_upper(key.name);
            if(!unlock_door(door_name)) {
                std::cout << "!! door " << door_name << " can't be unlocked." << std::endl;
            }
        }

        inline long unsigned int count_options(const std::unordered_set<Door, hashBase>& available_doors, 
        const std::unordered_set<Key, hashBase>& available_keys) {
            long unsigned int counter = 0;
            counter = available_doors.size() + available_keys.size();
            return counter;
        }

        bool more_options(const std::unordered_set<Door, hashBase>& available_doors, const std::unordered_set<Key, hashBase>& available_keys) {
            return (count_options(available_doors, available_keys) > 1);
        }

        Position get_enterence_position() {
            return enterence_position;
        }

        void set_enterence_position(const Position& position) {
            path.push_back(enterence_position);
            enterence_position = position;
        }

        void process_row(const int& row_number, const std::string& row) {
            int column = 0;
            std::for_each(row.begin(), row.end(), [&] (auto const& el) {
                Position position(column, row_number);
                std::string tmp(1, el);
                if(el == '.') {
                    path.push_back(position);
                } else if(el == '#') {
                    wall.push_back(position);
                } else if(el == '@') {
                    set_enterence_position(position);
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

        bool is_wall(const Position& position) const {
            return std::find(wall.begin(), wall.end(), position) != wall.end();
        }

        bool is_path(const Position& position) const {
            return std::find(path.begin(), path.end(), position) != path.end();
        }

        bool is_locked_door(const Position& position) const {
            for(auto& door:doors) {
                if(door.position == position) {
                    return door.is_locked();
                }
            }
            return false;
        }

        bool is_key(const Position& position) const {
            for(const auto& key:keys) {
                if(key.position == position) {
                    return true;
                }
            }
            return false;
        }

        inline bool is_unlocked(const Door& door) {
            return !door.is_locked();
        }

        std::unordered_set<Door, hashBase> get_available_doors() {
            std::unordered_set<Door, hashBase> available_doors;
            std::for_each(doors.begin(), doors.end(), [&](auto const& el) {
                if(is_reachable(el.position) && is_unlocked(el)) {
                    available_doors.insert(el);
                } else {
                    std::cout << "vrata " << el.name << " su zakljucana ili nisu dohvatljiva" << std::endl;
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

        Key get_key_at_position(const Position& position) const {
            for(const auto& key:keys) {
                if(key.position == position) {
                    return key;
                }
            }
            return Key("-1", Position(-1, -1));
        }

        int get_distance_from_enterence(const Position& position) const {
            if(distance.find(position) != distance.end()) {
                return distance.at(position);
            }
            return -1;
        }

        void set_distance_from_enterence(const Position& position, const int& steps_num) {
            distance[position] = steps_num;
        }

    private:  
        std::unordered_map<Position, int, MyHashFunction> distance;

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
        // (y, x)
        Position enterence_position = Position(-1, -1);
}; 

// TODO: fix this function so there it returns true in case when key "d" is the only one that is not picked up

// this function checks if there is a path to some position, but only using path, not picking up keys, opening doors
int Field::check_if_there_is_a_path(Field field, const Position& position, Position imaginary_enterence, 
                                    const Position& former_position) {
    if(imaginary_enterence == position) {
        return 1;
    }

    Position position_up = imaginary_enterence.get_modified_position(0, -1);
    Position position_down = imaginary_enterence.get_modified_position(0, 1);
    Position position_right = imaginary_enterence.get_modified_position(1, 0);
    Position position_left = imaginary_enterence.get_modified_position(-1, 0);

    std::array<Position, 4> neighbours = {position_up, position_down, position_left, position_right}; 
    
    for(auto const neighbour : neighbours) {
        if(former_position != neighbour && field.is_path(neighbour)) {
            int num_of_steps = field.check_if_there_is_a_path(field, position, neighbour, imaginary_enterence);
            if(num_of_steps > 0) {
                distance[position] = num_of_steps + 1;
                return num_of_steps + 1;
            }
        } else if(neighbour == position) { // if neighbour is destination
            return 1;
        }
    }
    // none of the neighbours can reach destination
    return 0;
}

template <typename T>
inline bool explored(std::unordered_set<T, hashBase> elements) {
    return elements.empty();
}

// TODO: distance_between_elements is not ok, i.e. in case when enterence is on position "E", and 
// has to calculate distance from "d"
int distance_from_enterence(const Field& field, const Position& position) {
    return field.get_distance_from_enterence(position);
}

int distance_between_elements(const Position& position_start, const Position& position_end) {
    const int distance_x = abs(position_end.x - position_start.x);
    const int distance_y = abs(position_end.y - position_start.y);
    std::cout << "premjestanje: udaljenost izmedju " << position_start << " i " << 
    position_end << " je " << distance_x + distance_y << std::endl;
    return distance_x + distance_y;
}

int search_and_count(Field field, Position new_enterence_position, bool is_key_position) {
    const int initial_value = 1;

    if(explored(field.doors) && explored(field.keys)) {
        return initial_value; // or 1? 
    }
    
    Position old_enterence_position = field.get_enterence_position();
    int distance = 0;
    if(old_enterence_position != new_enterence_position) {
        distance = field.get_distance_from_enterence(new_enterence_position);
        if(distance == -1) {
            distance = distance_between_elements(new_enterence_position, old_enterence_position);
        }
        field.set_enterence_position(new_enterence_position);
        if(is_key_position) {
            field.pick_up_key(new_enterence_position);
        } else {
            field.enter_the_door(new_enterence_position);
        }
    }

    // available doors are only the one that are unlocked

    std::unordered_set<Door, hashBase> available_doors = field.get_available_doors();
    std::unordered_set<Key, hashBase> available_keys = field.get_available_keys();

    int min = -1;
    int number_of_moves;

    if(available_doors.empty() && available_keys.empty()) {
        std::cout << "nema ni vrata, ni kljuceva" << std::endl;
    } else {
        for(const auto& door : available_doors) {
        std::cout << "istrazuju se vrata: " << door.name << std::endl;
        number_of_moves = search_and_count(field, door.position, false);
            if(number_of_moves < min || min == -1) {
                min = number_of_moves;
            } 
        }

        for(const auto& key : available_keys) {
            std::cout << "istrazuje se kljuc: " << key.name << std::endl;
            number_of_moves = search_and_count(field, key.position, true);
            if(number_of_moves < min || min == -1) {
                min = number_of_moves;
            } 
        }
    }

    return min == -1 ? distance : distance + min;
}

int get_shortest_path() {
    const std::string file_name = "day18.txt";
    Field field(file_name);
    std::cout << "pocetak" << std::endl;
    return search_and_count(field, field.get_enterence_position(), false);
}

int main() {
    int shortest_path = get_shortest_path();
    std::cout << "shortest path = " << shortest_path << std::endl;
    return 0;
}