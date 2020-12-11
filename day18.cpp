#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <memory>
#include <map>
#include <unordered_set>
#include <bitset>
#include <limits>
#include <queue>
#include <set>
#include <algorithm>

class Element 
{
    public:
        Element() = default;
        Element(int _y, int _x) : x(_x), y(_y) {}
        Element(int _y, int _x, char c) : x(_x), y(_y), symbol(c) {}
        int x, y;
        char symbol;
};

inline bool operator==(const Element& el1, const Element& el2) { return el1.x == el2.x && el1.y == el2.y; }
inline bool operator!=(const Element& el1, const Element& el2) { return !(el1 == el2); }

class Door : public Element 
{
    public:
        Door() = default;
        Door(int _y, int _x) : Element(_x, _y, '-') {}
        Door(int _y, int _x, char c) : Element(_x, _y, c) {}
};

class Passage : public Element 
{
    public:
        Passage(int _y, int _x)  : Element(_x, _y, '.') {}
        Passage() = default;
};

class Key : public Element 
{
    public:
        Key(int _y, int _x, char c) : Element(_x, _y, c) {}
        Key(char c) 
        {
            symbol = c;
        }
        Key() = default;
};

inline bool operator==(const Key& el1, const Key& el2) { return el1.symbol == el2.symbol; }

struct KeyHasher 
{
    std::size_t operator()(const Key& k) const 
    {
        using std::size_t;
        using std::hash;
        using std::string;
        return hash<char>()(k.symbol);
    }
};

class Board 
{
    public:
        std::vector<Passage> passages;
        std::vector<std::vector<Element>> all_elements;
        std::unordered_map<Key, Door, KeyHasher> map;
        Element current_position;

        // this function returns upper neighbour of current position
        Element up() 
        {
            return Element(current_position.y - 1, current_position.x);
        }

        // this function returns neighbour under the current position
        Element under() 
        {
            return Element(current_position.y + 1, current_position.x);
        }

        // this function returns left neighbour of current position
        Element left() 
        {
            return Element(current_position.y, current_position.x - 1);
        }

        // this function returns right neighbour of current position
        Element right() 
        {
            return Element(current_position.y, current_position.x + 1);
        }

        bool is_passable(Element el) 
        {
            for(Passage pass : passages) 
            {
                if(pass.x == el.x && pass.y == el.y) return true;
            }
            return is_key(el);
        }

        bool is_key(Element el) 
        {
            for(std::pair<Key, Door> pair : map) 
            {
                if(pair.first.x == el.x && pair.first.y == el.y) return true;
            }
            return false;
        }

        void print_passages() 
        {
            std::cout << "Passages:\n";
            for(Passage pass : passages) std::cout << "x = " << pass.x << ", y = " << pass.y << '\n';
        }

        void print_keys_and_doors() 
        {
            std::cout << "Keys and Doors:\n";
            for(std::pair<const Key, Door>& pair : map) 
            {
                std::cout << "Key.x = " << pair.first.x << ", Key.y = " << pair.first.y << ", Key.symbol = " << pair.first.symbol << '\n';
                std::cout << "Door.x = " << pair.second.x << ", Door.y = " << pair.second.y << '\n';
            }
        }

        // TODO: update all_elements when current_position changes
        void print_board() 
        {
            int num_of_columns = all_elements[0].size();
            for(int row = 0; row < all_elements.size(); row++) 
            {
                for(int column = 0; column < num_of_columns; column++) 
                {
                    std::cout << (all_elements[row][column]).symbol;
                }
                std::cout << '\n';
            }
        }
    private:
};

void build_board(const std::string& file_name, Board& board) 
{
    std::ifstream ifs (file_name, std::ifstream::in);
    char c = ifs.get();
    int row = 0, column = 0;
    std::vector<Element> row_elements;
    while (ifs.good()) 
    {
        Element el(row, column, c);
        if(c == '.') { // path
            board.passages.push_back(Passage(row, column));
        } else if(c == '@') { // entrance
            board.current_position = el;
            board.passages.push_back(Passage(row, column)); // entrance is on the passage
        } else if(c >= 'a' && c <= 'z') { // key
            Key key_tmp(row, column, c);
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
                board.map[c + 32] = Door(row, column, c);
            } else {
                // key doesn't exist
                board.map.emplace(c + 32, Door(row, column, c));
            }
        } else if(c != '#') { // new row
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

bool only_way(std::vector<bool> passages, int index) 
{
    for(int i = 0; i < passages.size(); i++) 
    {
        if(i != index && passages[i] == true) return false;
    }
    return true;
}

class State 
{
    public:
        State(int y, int x, int s, long long v) : curr_x(x), curr_y(y), steps(s), visited(v) {}
        int curr_x, curr_y, steps;
        long long visited;
};

bool check_in_map(int x, int y, int lines, int columns)
{
    bool res = x >= 0 && y >= 0 && x < lines && y < columns;
    std::cout << "in map = " << (res ? "true" : "false") << '\n';
    return res;
}

bool is_wall(char c)
{
    std::cout << "is wall = " << ((c == '#') ? "true" : "false") << '\n';
    return c == '#';
}

bool is_repeated(int x, int y, long long visited, std::set<std::tuple<int, int, long long>> repeated)
{
    bool result = repeated.find(std::make_tuple(y, x, visited)) != repeated.end();
    std::cout << "repeated = " << (result == true ? "true" : "false") << '\n'; 
    return result;
}

/**
 * BTS algorithm for calculating the shortest path
 */
int collect_keys(Board board, Element current_position) 
{
    int min = std::numeric_limits<int>::max();
    constexpr int num_of_letters = 26;
	std::bitset<num_of_letters> visited;
    std::set<std::tuple<int, int, long long>> repeated;
    std::queue<State> states;
    std::vector<int> x_delta = {0, 0, -1, 1};
    std::vector<int> y_delta = {-1, 1, 0, 0};
    std::cout << "initial state: x = " << current_position.x << ", y = " << current_position.y << '\n';
    states.push(State(current_position.y, current_position.x, 0, visited.to_ullong()));
    while (!states.empty())
    {
        State curr = states.front();
        std::cout << "curr.x = " << curr.curr_x << ", curr.y = " << curr.curr_y << '\n';
        states.pop();
        visited = curr.visited;
        if(visited.all()) 
        {
            return curr.steps;
        }
        for(int i = 0; i < 4; i++)
        {
            if(!check_in_map(curr.curr_y + y_delta[i], curr.curr_x + x_delta[i], board.all_elements.size(), board.all_elements[0].size())
            || is_wall(board.all_elements[curr.curr_y + y_delta[i]][curr.curr_x + x_delta[i]].symbol)
            || is_repeated(curr.curr_y + y_delta[i], curr.curr_x + x_delta[i], curr.visited, repeated)) 
            {
                std::cout << "out of range\n";
                continue;
            }
            if(isupper(board.all_elements[curr.curr_y + y_delta[i]][curr.curr_x + x_delta[i]].symbol) && 
            (!visited[board.all_elements[curr.curr_y + y_delta[i]][curr.curr_x + x_delta[i]].symbol - 'A']))
            {
                std::cout << "upper\n";
                continue;
            }
            if(islower(board.all_elements[curr.curr_y + y_delta[i]][curr.curr_x + x_delta[i]].symbol))
            {
                std::cout << "lower\n";
                visited[board.all_elements[curr.curr_y + y_delta[i]][curr.curr_x + x_delta[i]].symbol - 'a'] = true;
            }
            std::cout << "**curr.x = " << curr.curr_x + x_delta[i] << ", curr.y = " << curr.curr_y + y_delta[i] << '\n';
            states.push(State(curr.curr_y + y_delta[i], curr.curr_x + x_delta[i], curr.steps + 1, visited.to_ullong()));
            repeated.insert(std::make_tuple(curr.curr_y + y_delta[i], curr.curr_x + x_delta[i], visited.to_ullong()));
            if(islower(board.all_elements[curr.curr_y + y_delta[i]][curr.curr_x + x_delta[i]].symbol))
            {
                visited[board.all_elements[curr.curr_y + y_delta[i]][curr.curr_x + x_delta[i]].symbol - 'a'] = false;
            }
        }
    }
    return min;
}

int collect_keys(Board board) 
{
    return collect_keys(board, board.current_position);
}

int main() 
{
    Board board;
    auto start = std::chrono::high_resolution_clock::now(); 
    build_board("day18.txt", board);
    auto stop = std::chrono::high_resolution_clock::now(); 
    std::cout << "start\n";
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 
    std::cout << duration.count() << std::endl; 
    int shortest_path = collect_keys(board);
    std::cout << "Shortest path = " << shortest_path << '\n';
}
