#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

class Position {
    public:
        int x, y; // 0 is first element inside od #
};

class Base {
    public:
        Base() {}
        std::string name;
        Position position;
        bool is_reachable(Position enterence_position) {
            // check if rechable
        }
};

bool operator==(const Base& x, const Base& y)
{
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
        inline void add_door(Door door) {
            doors.insert(door);
        }

        inline void add_key(Key key) {
            keys.insert(key);
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

        std::unordered_set<Door, hashBase> doors;
        std::unordered_set<Key, hashBase> keys;

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
    // open file
    // read line by line and save in field
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