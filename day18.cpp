#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>

class Element {
    public:
        Element() = default;
        Element(int _x, int _y, char c) : x(_x), y(_y), symbol(c) {}
        Element(const Element& el) { x = el.x; y = el.y; symbol = el.symbol; }
        Element& operator=(const Element& el) { x = el.x; y = el.y; symbol = el.symbol; return *this; }
        Element& operator=(Element&& el) { x = el.x; y = el.y; symbol = el.symbol; return *this; }
        int x, y;
        char symbol;
        bool path; // true for keys, unlocked doors and passages, otherwise false (for stone walls and locked doors)
};

inline bool operator==(const Element& el1, const Element& el2) { return el1.x == el2.x && el1.y == el2.y; }

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
        // TODO: add elements when board initialized, remove elements when collected
        std::list<Element> uncollected_elements;
        // all elements contains uncollected and collected elements
        std::vector<std::vector<Element>> all_elements;
        std::vector<Door> doors;
        std::vector<Key> keys;
        std::vector<Passage> passages;
        std::vector<StoneWall> stone_walls;
        std::unordered_map<Key, Door, KeyHasher> map;
        Element entrance;

        bool get_element(int x, int y, Element& element) {
            for(const Element& el : uncollected_elements) {
                if(el.x == x && el.y == y) {
                    element = el;
                    return true;
                }
            }    
            return false;
        }

        bool is_path(Element element) {
            if(get_element(element.x, element.y, element)) return element.path;
            return false;
        }

        // move recursively the entrance to the destination
        bool move_to(Element destination) {
            if(are_neighbours(entrance, destination)) {
                // just move
                entrance = destination;
                return true;
            }
            // TODO: recursively go to the destination
            return false;
        }

        bool all_elements_collected() {
            return uncollected_elements.size() == 0; // there are no elements on the board
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
        bool are_neighbours(const Element& source, const Element& destination) {
            if(std::abs(source.x - destination.x) > 1 || std::abs(source.y - destination.y) > 1) {
                return false;
            }
            // if diagonal move return false, otherwise true
            return !(std::abs(source.x - destination.x) == 1 && std::abs(source.y - destination.y) == 1);
        }
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
        } else if(c == '@') { // entrance
            el.path = true;
            board.entrance = el;
        } else if(c == '#') { // wall
            el.path = false;
        } else if(c >= 'a' && c <= 'z') { // key
            el.path = true;
        } else if(c >= 'A' && c <= 'Z') { // door
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
        board.uncollected_elements.push_back(el);
        row_elements.push_back(el);
        c = ifs.get();
    }
    // add last row
    board.all_elements.push_back(row_elements);
    ifs.close();
}

// x - column, y - row
int collect_keys(Board board, Element entrance, int& number_of_steps) { // entrance is send so step can be reverted
    board.print_board();
    // special case : all elements collected
    if(board.all_elements_collected()) {
        // TODO: think about what to do in the last step and implement it 
        std::cout << "all elements on the board are collected\n";
    }
    // go recursively in all directions (up, down, right and left) if there is a path, unlocked door or a key
    Element neighbour;
    board.get_element(entrance.x, entrance.y - 1, neighbour); 
    // TODO: if neighbour is key, unlock a door associated with it
    if(board.is_path(neighbour)) { // up
        std::cout << "visit neighbour up\n";
        // TODO: add step counter and return value
        board.move_to(neighbour);
        board.uncollected_elements.remove(neighbour); // remove neighbour from all elements because it's collected
        collect_keys(board, neighbour, number_of_steps);
        board.uncollected_elements.push_back(neighbour); // revert back like it has never been collected
        board.move_to(entrance); // move back (revert)
    } 
    board.get_element(entrance.x, entrance.y + 1, neighbour);
    // TODO: if neighbour is key, unlock a door associated with it
    if(board.is_path(neighbour)) { // down
        std::cout << "visit neighbour down\n";
        // TODO: add step counter and return value
        board.move_to(neighbour);
        board.uncollected_elements.remove(neighbour); // remove neighbour from all elements because it's collected
        collect_keys(board, neighbour, number_of_steps);
        board.uncollected_elements.push_back(neighbour); // revert back like it has never been collected
        board.move_to(entrance); // move back (revert)
    } 
    board.get_element(entrance.x - 1, entrance.y, neighbour);
    // TODO: if neighbour is key, unlock a door associated with it
    if(board.is_path(neighbour)) { // left
        std::cout << "visit neighbour left\n";
        // TODO: add step counter and return value
        board.move_to(neighbour);
        board.uncollected_elements.remove(neighbour); // remove neighbour from all elements because it's collected
        collect_keys(board, neighbour, number_of_steps);
        board.uncollected_elements.push_back(neighbour); // revert back like it has never been collected
        board.move_to(entrance); // move back (revert)
    } 
    board.get_element(entrance.x + 1, entrance.y, neighbour);
    // TODO: if neighbour is key, unlock a door associated with it
    if(board.is_path(neighbour)) { // right
        std::cout << "visit neighbour right\n";
        // TODO: add step counter and return value
        board.move_to(neighbour);
        board.uncollected_elements.remove(neighbour); // remove neighbour from all elements because it's collected
        collect_keys(board, neighbour, number_of_steps);
        board.uncollected_elements.push_back(neighbour); // revert back like it has never been collected
        board.move_to(entrance); // move back (revert)
    } 
    return 0; // TODO: return valid value
}

int collect_keys(const Board& board) {
    int number_of_steps = 0;
    return collect_keys(board, board.entrance, number_of_steps);
}

int main() {
    Board board;
    build_board("day18.txt", board);
    int shortest_path = collect_keys(board);
    std::cout << "Shortest path = " << shortest_path << '\n';
}
