#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <map>
#include <unordered_set>
#include <algorithm>

class Element {
    public:
        Element() = default;
        Element(int _x, int _y) : x(_x), y(_y) {}
        Element(int _x, int _y, char c) : x(_x), y(_y), symbol(c) {}
        int x, y;
        char symbol;
};

inline bool operator==(const Element& el1, const Element& el2) { return el1.x == el2.x && el1.y == el2.y; }
inline bool operator!=(const Element& el1, const Element& el2) { return !(el1 == el2); }

class Door : public Element {
    public:
        Door() = default;
        Door(int _x, int _y) : Element(_x, _y, '-') {}
        Door(int _x, int _y, char c) : Element(_x, _y, c) {}
};

class Passage : public Element {
    public:
        Passage(int _x, int _y) : Element(_x, _y, '.') {}
        Passage() = default;
};

class Key : public Element {
    public:
        Key(int _x, int _y, char c) : Element(_x, _y, c) {}
        Key(char c) {
            symbol = c;
        }
        Key() = default;
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
        Element current_position;
        
        // this function returns upper neighbour of current position
        Element up() {
            return Element(current_position.x, current_position.y - 1);
        }

        // this function returns neighbour under the current position
        Element under() {
            return Element(current_position.x, current_position.y + 1);
        }

        // this function returns left neighbour of current position
        Element left() {
            return Element(current_position.x - 1, current_position.y);
        }

        // this function returns right neighbour of current position
        Element right() {
            return Element(current_position.x + 1, current_position.y);
        }

        bool is_passable(Element el) {
            for(std::pair<Key, Door> pair : map) {
                if(pair.first.x == el.x && pair.first.y == el.y) return true;
            }
            for(Passage pass : passages) {
                if(pass.x == el.x && pass.y == el.y) return true;
            }
            return false;
        }

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

        // TODO: update all_elements when current_position changes
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
            board.passages.push_back(Passage(column, row));
        } else if(c == '@') { // entrance
            board.current_position = el;
        } else if(c == '#') { // wall
            std::cout << "wall\n";
        } else if(c >= 'a' && c <= 'z') { // key
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

int collect_keys(Board board, Element current_position, Element previous_position, int& number_of_steps) {
    std::cout << "x = " << current_position.x << ", y = " << current_position.y << " is current position.\n";
    if(Element up = board.up(); up != previous_position && board.is_passable(up)) {
        std::cout << "x = " << up.x << ", y = " << up.y << " is passable.\n";
        number_of_steps++;
        board.current_position = up;
        collect_keys(board, up, current_position, number_of_steps);
    }
    if(Element under = board.under(); under != previous_position && board.is_passable(under)) {
        std::cout << "x = " << under.x << ", y = " << under.y << " is passable.\n";
        number_of_steps++;
        board.current_position = under;
        collect_keys(board, under, current_position, number_of_steps);
    }
    if(Element left = board.left(); left != previous_position && board.is_passable(left)) {
        std::cout << "x = " << left.x << ", y = " << left.y << " is passable.\n";
        number_of_steps++;
        board.current_position = left;
        collect_keys(board, left, current_position, number_of_steps);
    }
    if(Element right = board.right(); right != previous_position && board.is_passable(right)) {
        std::cout << "x = " << right.x << ", y = " << right.y << " is passable.\n";
        number_of_steps++;
        board.current_position = right;
        collect_keys(board, right, current_position, number_of_steps);
    }
    return -1;
}

int collect_keys(const Board& board) {
    int number_of_steps = 0;
    return collect_keys(board, board.current_position, board.current_position, number_of_steps);
}

int main() {
    Board board;
    build_board("day18.txt", board);
    int shortest_path = collect_keys(board);
    std::cout << "Shortest path = " << shortest_path << '\n';
}
