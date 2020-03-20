#include <cstdio>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <iostream>       
#include <string>         
#include <locale>
#include <cctype>

class Position {
    public:
        Position() = default;
        Position(int _x, int _y) : x(_x), y(_y) {}
        int x, y;
        Position get_modified_position(int x_offset, int y_offset) const {
            return Position(x + x_offset, y + y_offset);
        }
};

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

        bool check_if_there_is_a_path(Field field, const Position& position, Position imaginary_enterence,
                             bool key_processing, const Key& key = Key("", Position(-1,-1)));

        bool is_reachable(const Position& position) {
            std::cout << "checking if position is reachable: (" << position.x << ", " << position.y << ")" << std::endl;  
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
            return check_if_there_is_a_path(*this, position, enterence_position, false);
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
                    break;
                }
            }
            path.push_back(position);
            if(!unlock_door(str_to_upper(name))) {
                std::cout << "!! door can't be unlocked." << std::endl;
            }
        }

        void pick_up_key(const Key& key) {
            keys.erase(key);
            path.push_back(key.position);
            if(!unlock_door(str_to_upper(key.name))) {
                std::cout << "!! door can't be unlocked." << std::endl;
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
                bool reachable = is_reachable(el.position);
                bool unlocked = is_unlocked(el);
                if(!reachable) std::cout << "vrata " << el.name << " nisu dohvatljiva" << std::endl;
                if(!unlocked) std::cout << "vrata " << el.name << " su zakljucana" << std::endl;
                if(is_reachable(el.position) && is_unlocked(el)) {
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

        Key get_key_at_position(const Position& position) const {
            for(const auto& key:keys) {
                if(key.position == position) {
                    return key;
                }
            }
            return Key("-1", Position(-1, -1));
        }

    private:  

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

bool Field::check_if_there_is_a_path(Field field, const Position& position, Position imaginary_enterence,
                             bool key_processing, const Key& key) {
    
    if(key_processing) {
        field.pick_up_key(key);
    }

    if(imaginary_enterence == position) {
        // enterence can reach position
        return true;
    }

    // check if there is possible path up, down, right or left
    Position position_up = imaginary_enterence.get_modified_position(0, 1);
    Position position_down = imaginary_enterence.get_modified_position(0, -1);
    Position position_right = imaginary_enterence.get_modified_position(-1, 0);
    Position position_left = imaginary_enterence.get_modified_position(1, 0);

    std::array<Position, 4> neighbours = {position_up, position_down, position_left, position_right}; 
    
    for(auto const neighbour : neighbours) {
        // if neighbour is path or unlocked door or key call check_if_there is_a_path(neighbout)
        // ako je rez true, return true
        if(field.is_wall(neighbour) || field.is_locked_door(neighbour)) {
            continue;
        } 
        if(field.is_path(neighbour)) {
            // if path -> recursive call
            check_if_there_is_a_path(field, position, neighbour, false);
        } else { // key
        // if key, pick up it and convert it to path and unlock door, but only in next call..in this call
        // nothing changes
            Key tmp_key = field.get_key_at_position(neighbour);
            check_if_there_is_a_path(field, position, neighbour, true, key);
            // add third arg where it will be indicated if door need to be unlocked, key picked up etc.
        }
    }
    // none of the neighbours can reach destination
    return false;
}

template <typename T>
inline bool explored(std::unordered_set<T, hashBase> elements) {
    return elements.empty();
}

int distance_between_elements(const Position& position_start, const Position& position_end) {
    const int distance_x = abs(position_end.x - position_start.x);
    const int distance_y = abs(position_end.y - position_start.y);
    std::cout << "udaljenost izmedju (" << position_end.x << "," << position_end.y << ") i (" << 
    position_start.x << "," << position_start.y << ") je " << distance_x + distance_y << std::endl;
    return distance_x + distance_y;
}

int search_and_count(Field field, Position new_enterence_position, bool is_key_position) {
    const int initial_value = 1;

    if(explored(field.doors) && explored(field.keys)) {
        return initial_value; // or 1? 
    }
    
    Position old_enterence_position = field.get_enterence_position();
    int basic_distance;
    if(old_enterence_position != new_enterence_position) {
        std::cout << "premjestanje" << std::endl;
        basic_distance = distance_between_elements(old_enterence_position, new_enterence_position);
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
    int min = 99999;
    int number_of_moves;
    bool return_initial_value = true;

    for(const auto& door : available_doors) {
        std::cout << "istrazuju se vrata: " << door.name << std::endl;
        number_of_moves = search_and_count(field, door.position, false);
        if(number_of_moves < min) {
            min = number_of_moves + distance_between_elements(door.position, new_enterence_position) + basic_distance;
            return_initial_value = false;
        } 
    }

    for(const auto& key : available_keys) {
        std::cout << "istrazuje se kljuc: " << key.name << std::endl;
        number_of_moves = search_and_count(field, key.position, true);
        if(number_of_moves < min) {
            min = number_of_moves + distance_between_elements(key.position, new_enterence_position) + basic_distance;
            return_initial_value = false;
        } 
    }

    return return_initial_value ? initial_value : number_of_moves; // return something
}

int get_shortest_path() {
    const std::string file_name = "day18.txt";
    Field field(file_name);
    return search_and_count(field, field.get_enterence_position(), false);
}

int main() {
    int shortest_path = get_shortest_path();
    std::cout << "shortest path = " << shortest_path << std::endl;
    return 0;
}