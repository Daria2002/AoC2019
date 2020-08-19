#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>

class Element {
    public:
        Element() = default;
        Element(int _x, int _y) : x(_x), y(_y) {}
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
        bool is_path(const Element& element) {
            for(const Element& el : all_elements) {
                if(el.x == element.x && el.y == element.y) {
                    return el.path;
                }
            }
            return false;
        }
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

// x - column, y - row
int collect_keys(Board board, Element entrance) {
    // go recursively in all directions (up, down, right and left) if there is a path, unlocked door or a key
    Element neighbour = Element(entrance.x, entrance.y - 1); 
    if(board.is_path(neighbour)) { // up
        // TODO: move up on board
        collect_keys(board, neighbour);
    } 
    neighbour = Element(entrance.x, entrance.y + 1); 
    if(board.is_path(neighbour)) { // down
        // TODO: move down on board
        collect_keys(board, neighbour);
    } 
    neighbour = Element(entrance.x - 1, entrance.y); 
    if(board.is_path(neighbour)) { // left
        // TODO: move left on board
        collect_keys(board, neighbour);
    } 
    neighbour = Element(entrance.x + 1, entrance.y); 
    if(board.is_path(neighbour)) { // right
        // TODO: move right on board
        collect_keys(board, neighbour);
    } 
}

int collect_keys(const Board& board) {
    return collect_keys(board, board.entrance);
}

int main() {
    Board board;
    build_board("day18.txt", board);
    int shortest_path = collect_keys(board);
    std::cout << "Shortest path = " << shortest_path << '\n';
}
