#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <map>
#include <unordered_set>

class Element {
    public:
        Element() = default;
        Element(int _x, int _y, char c) : x(_x), y(_y), symbol(c) {}
        int x, y;
        char symbol;
        bool path; // true for keys, unlocked doors and passages, otherwise false (for stone walls and locked doors)
};

inline bool operator==(const Element& el1, const Element& el2) { return el1.x == el2.x && el1.y == el2.y; }

class Door : public Element {
    public:
        Door(int _x, int _y) : Element(_x, _y, '-') {}
        Door(int _x, int _y, char c) : Element(_x, _y, c) {}
        Door() {
            path = false;
        }
        void unlock() {
            path = true;
        }
};

class Passage : public Element {
    public:
        Passage(int _x, int _y) : Element(_x, _y, '.') {}
        Passage() {
            path = true;
        }
};

class Key : public Element {
    public:
        Key(int _x, int _y, char c) : Element(_x, _y, c) {}
        Key(char c) {
            symbol = c;
        }
        Key() {
            path = true;
        }
};

inline bool operator==(const Key& el1, const Key& el2) { return el1.symbol == el2.symbol; }

struct KeyHasher {
    std::size_t operator()(const Key& k) const {
        using std::size_t;
        using std::hash;
        using std::string;
        return hash<char>()(k.symbol);
    }
};

class Board {
    public:
        std::vector<Passage> passages;
        std::vector<std::vector<Element>> all_elements;
        std::unordered_map<Key, Door, KeyHasher> map;
        Element entrance;
        
        void print_passages() {
            std::cout << "Passages:\n";
            for(Passage pass : passages) std::cout << "x = " << pass.x << ", y = " << pass.y << '\n';
        }

        void print_keys_and_doors() {
            std::cout << "Keys and Doors:\n";
            for(std::pair<const Key, Door>& pair : map) {
                std::cout << "Key.x = " << pair.first.x << ", Key.y = " << pair.first.y << ", Key.symbol = " << pair.first.symbol << '\n';
                std::cout << "Door.x = " << pair.second.x << ", Door.y = " << pair.second.y << '\n';
            }
        }

        // TODO: update all_elements when entrance moves
        void print_board() {
            int num_of_columns = all_elements[0].size();
            for(int row = 0; row < all_elements.size(); row++) {
                for(int column = 0; column < num_of_columns; column++) {
                    std::cout << (all_elements[row][column]).symbol;
                }
                std::cout << '\n';
            }
        }
    private:
};

void build_board(const std::string& file_name, Board& board) {
    std::ifstream ifs (file_name, std::ifstream::in);
    char c = ifs.get();
    int row = 0, column = 0;
    std::vector<Element> row_elements;
    while (ifs.good()) {
        Element el(column, row, c);
        if(c == '.') { // path
            el.path = true;
            board.passages.push_back(Passage(column, row));
        } else if(c == '@') { // entrance
            el.path = true;
            board.entrance = el;
        } else if(c == '#') { // wall
            el.path = false;
        } else if(c >= 'a' && c <= 'z') { // key
            el.path = true;
            Key key_tmp(column, row, c);
            std::unordered_map<Key, Door, KeyHasher>::iterator it = board.map.find(c);
            if(board.map.find(c) != board.map.end()) {
                Door tmp = board.map[c];
                board.map.erase(key_tmp);
                board.map.emplace(key_tmp, tmp);
            } else {
                board.map[key_tmp] = Door(-1, -1);
            }
        } else if(c >= 'A' && c <= 'Z') { // door
            if(board.map.find(c + 32) != board.map.end()) {
                // key already exists
                board.map[c + 32] = Door(column, row, c);
            } else {
                // key doesn't exist
                board.map.emplace(c + 32, Door(column, row, c));
            }
            el.path = false;
        } else { // new row
            row++;
            column = 0; // initialize column to 0 and continue
            c = ifs.get();
            board.all_elements.push_back(row_elements);
            row_elements.clear();
            continue;
        }
        column++;
        row_elements.push_back(el);
        c = ifs.get();
    }
    // add last row
    board.all_elements.push_back(row_elements);
    ifs.close();
}

int collect_keys(Board board, Element entrance, int& number_of_steps) {
    // todo
}

int collect_keys(const Board& board) {
    int number_of_steps = 0;
    return collect_keys(board, board.entrance, number_of_steps);
}

int main() {
    Board board;
    build_board("day18.txt", board);
    // int shortest_path = collect_keys(board);
    // std::cout << "Shortest path = " << shortest_path << '\n';
}
