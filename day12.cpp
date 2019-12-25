#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream> 
#include <cmath>

#define NUMBER_OF_STEPS 1000

class Moon {

    public:
        int _x, _y, _z, _v_x, _v_y, _v_z;
        Moon(int x, int y, int z) : _x(x), _y(y), _z(z)
        {
            _v_x = _v_y = _v_z = 0;
        }

        void update_velocities(std::vector<Moon> moon) {
            for(int i = 0; i < moon.size(); i++) {
                if(moon[i]._x < _x) {
                    _v_x--;
                } else if(moon[i]._x > _x) {                    
                    _v_x++;
                }   

                if(moon[i]._y < _y) {
                    _v_y--;
                } else if(moon[i]._y > _y) {
                    _v_y++;
                }

                if(moon[i]._z < _z) {
                    _v_z--;
                } else if(moon[i]._z > _z) {
                    _v_z++;
                }
            }
        }

        void update_positions() {
            _x = _x + _v_x;
            _y = _y + _v_y;
            _z = _z + _v_z;
        }
};

std::string get_str_between_two_str(const std::string s,
        const std::string start_delim,
        const std::string stop_delim)
{
    unsigned first_delim_pos = s.find_first_of(start_delim);
    unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
    // param1 = string containing characters to locate
    // param2 = index of character to search from
    unsigned last_delim_pos = s.find_first_of(stop_delim, end_pos_of_first_delim);

    // param1 = index of first character
    // param2 = number of characters in substring
    return s.substr(end_pos_of_first_delim,
            last_delim_pos - end_pos_of_first_delim);
}

void get_initial_state(std::vector<Moon>& moons) {
    std::fstream file("./day12.txt");
    std::string line;

    std::array<int, 3> arr;

    while(getline(file, line)) {
        for(int i = 0; i < 3; i++) {
            if(i == 0 || i == 1) {
                arr[i] = std::stoi(get_str_between_two_str(line, "=", ","));
            } else {
                arr[i] = std::stoi(get_str_between_two_str(line, "=", ">"));
            }
            line = line.substr(line.find_first_of(",") + 1);
        }
        moons.push_back(Moon(arr.at(0), arr.at(1), arr.at(2)));
        arr.empty();
    }
}

int calculate_total_energy(std::vector<Moon> state) {
    std::array<int, 4> potential;
    // potential
    for(int i = 0; i < 4; i++) {
        potential[i] = std::abs(state[i]._x) + 
                       std::abs(state[i]._y) +
                       std::abs(state[i]._z);
        std::cout << "potential[" << i << "] = " << potential[i] << std::endl;
    }

    std::array<int, 4> kinetic;
    // kinetic
    for(int i = 0; i < 4; i++) {
        kinetic[i] = std::abs(state[i]._v_x) + 
                     std::abs(state[i]._v_y) +
                     std::abs(state[i]._v_z);
        std::cout << "kinetic[" << i << "] = " << kinetic[i] << std::endl;
    }

    std::array<int, 4> total;
    for(int i = 0; i < 4; i++) {
        total[i] = potential[i] * kinetic[i];
        std::cout << "total[" << i << "] = " << total[i] << std::endl;
    }

    // return sum of total;
    int sum_of_total = total[0] + total[1] + total[2] + total[3];
    std::cout << "sum of total = " << sum_of_total << std::endl;
    return sum_of_total;
}

int main() {
    // key is step, value is vector of four states (for each moon)
    std::map<int, std::vector<Moon>> moons;
    // get initial position, for step = 0
    get_initial_state(moons[0]);

    std::vector<Moon> state = moons[0];
    for(int i = 0; i < 4; i++)
    std::printf("<x=%d, y=%d, z=%d>         <v_x=%d, v_y=%d, v_z=%d>\n",
        state[i]._x, state[i]._y, state[i]._z, state[i]._v_x, state[i]._v_y, state[i]._v_z);
    // calculate for defined number of steps
    for(int i = 1; i < NUMBER_OF_STEPS+1; i++) {
        int j = 0;
        state.clear();

        std::cout << "korak = " << i << std::endl;

        std::for_each(moons[i-1].begin(), moons[i-1].end(), [&](Moon moon) {
            // help contains all moons except moon to update
            std::vector<Moon> help = moons[i-1];
            help.erase(help.begin() + j);
            j++;

            moon.update_velocities(help);
            moon.update_positions();
            state.push_back(moon);

            std::printf("<x=%d, y=%d, z=%d>         <v_x=%d, v_y=%d, v_z=%d>\n",
             moon._x, moon._y, moon._z, moon._v_x, moon._v_y, moon._v_z);
        });

        moons[i] = state;
    }

    int part1 = calculate_total_energy(state);
    std::cout << "part1 = " << part1 << std::endl;
}