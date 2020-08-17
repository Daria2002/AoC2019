#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

class Element {
    public:
        int x, y;
        bool path; // true for keys, unlocked doors and passages, otherwise false (for stone walls and locked doors)
};

class Door : public Element {
    public:
        Door() {
            path = false;
        }
        void unlock() {
            path = true;
        }
};

class StoneWall : public Element {
    public:
        StoneWall() {
            path = false;
        }
};

class Passage : public Element {
    public:
        Passage() {
            path = true;
        }
};

class Key : public Element {
    public:
        Key() {
            path = true;
        }
};

struct KeyHasher {
    std::size_t operator()(const Key& k) const {
        using std::size_t;
        using std::hash;
        using std::string;
        return ((hash<int>()(k.x) ^ (hash<int>()(k.y) << 1)) >> 1);
    }
};

class Board {
    public:
        std::vector<Element> all_elements;
        std::vector<Door> doors;
        std::vector<Key> keys;
        std::vector<Passage> passages;
        std::vector<StoneWall> stone_walls;
        std::unordered_map<Key, Door, KeyHasher> map;
        Element entrance;
};

void build_board(const std::string& file_name, Board& board) {
    std::ifstream ifs (file_name, std::ifstream::in);

    char c = ifs.get();

    while (ifs.good()) {
        std::cout << c;
        c = ifs.get();
    }

    // TODO : build board

    ifs.close();
}

int main() {
    Board board;
    build_board("day18.txt", board);
}
