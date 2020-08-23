#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>

class Element {
    public:
        Element() = default;
        Element(int _x, int _y) : x(_x), y(_y) {}
        Element(const Element& el) { x = el.x; y = el.y; }
        Element(Element&&) = default;
        Element& operator=(const Element&) = default;
        Element& operator=(Element&&) = default;
        int x, y;
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
        std::list<Element> all_elements; 
        std::vector<Door> doors;
        std::vector<Key> keys;
        std::vector<Passage> passages;
        std::vector<StoneWall> stone_walls;
        std::unordered_map<Key, Door, KeyHasher> map;
        Element entrance;
        bool is_path(Element& element) {
            for(const Element& el : all_elements) {
                if(el.x == element.x && el.y == element.y) {
                    return el.path;
                }
            }
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
            return all_elements.size() == 0; // there are no elements on the board
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

    while (ifs.good()) {
        std::cout << c;
        c = ifs.get();
    }

    // TODO : build board

    ifs.close();
}

// x - column, y - row
int collect_keys(Board board, Element entrance, int& number_of_steps) { // entrance is send so step can be reverted
    // special case : all elements collected
    if(board.all_elements_collected()) {
        // TODO: think about what to do in the last step and implement it 
    }
    // go recursively in all directions (up, down, right and left) if there is a path, unlocked door or a key
    Element neighbour = Element(entrance.x, entrance.y - 1); 
    if(board.is_path(neighbour)) { // up
        // TODO: add step counter and return value
        board.move_to(neighbour);
        board.all_elements.remove(neighbour); // remove neighbour from all elements because it's collected
        collect_keys(board, neighbour, number_of_steps);
        board.all_elements.push_back(neighbour); // revert back like it has never been collected
        board.move_to(entrance); // move back (revert)
    } 
    neighbour = Element(entrance.x, entrance.y + 1); 
    if(board.is_path(neighbour)) { // down
        // TODO: add step counter and return value
        board.move_to(neighbour);
        board.all_elements.remove(neighbour); // remove neighbour from all elements because it's collected
        collect_keys(board, neighbour, number_of_steps);
        board.all_elements.push_back(neighbour); // revert back like it has never been collected
        board.move_to(entrance); // move back (revert)
    } 
    neighbour = Element(entrance.x - 1, entrance.y); 
    if(board.is_path(neighbour)) { // left
        // TODO: add step counter and return value
        board.move_to(neighbour);
        board.all_elements.remove(neighbour); // remove neighbour from all elements because it's collected
        collect_keys(board, neighbour, number_of_steps);
        board.all_elements.push_back(neighbour); // revert back like it has never been collected
        board.move_to(entrance); // move back (revert)
    } 
    neighbour = Element(entrance.x + 1, entrance.y); 
    if(board.is_path(neighbour)) { // right
        // TODO: add step counter and return value
        board.move_to(neighbour);
        board.all_elements.remove(neighbour); // remove neighbour from all elements because it's collected
        collect_keys(board, neighbour, number_of_steps);
        board.all_elements.push_back(neighbour); // revert back like it has never been collected
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
