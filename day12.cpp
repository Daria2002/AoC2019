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
    }

    std::array<int, 4> kinetic;
    // kinetic
    for(int i = 0; i < 4; i++) {
        kinetic[i] = std::abs(state[i]._v_x) + 
                     std::abs(state[i]._v_y) +
                     std::abs(state[i]._v_z);
    }

    std::array<int, 4> total;
    for(int i = 0; i < 4; i++) {
        total[i] = potential[i] * kinetic[i];
    }

    // return sum of total;
    int sum_of_total = total[0] + total[1] + total[2] + total[3];
    return sum_of_total;
}

bool period(std::vector<long long int> values) {

    if(values.size() % 2 == 1 || values.size() < 2) {
        return false;
    } 

    for(long long int i = 0; i < values.size()/2; i++) {
        if(values[i] != values[values.size()/2 + i]) {
            return false;
        }
    }
    return true;
}

unsigned long long int lcm(std::vector<int> v) 
{ 
    // Find the maximum value in arr[] 
    int max_num = 0; 
    for (int i=0; i < v.size(); i++) 
        if (max_num < v[i]) 
            max_num = v[i]; 
  
    // Initialize result 
    unsigned long long int res = 1; 
  
    // Find all factors that are present in 
    // two or more array elements. 
    int x = 2;  // Current factor. 
    while (x <= max_num) 
    { 
        // To store indexes of all array 
        // elements that are divisible by x. 
        std::vector<int> indexes; 
        for (int j=0; j < v.size(); j++) 
            if (v[j]%x == 0) 
                indexes.push_back(j); 
  
        // If there are 2 or more array elements 
        // that are divisible by x. 
        if (indexes.size() >= 2) 
        { 
            // Reduce all array elements divisible 
            // by x. 
            for (int j=0; j<indexes.size(); j++) 
                v[indexes[j]] = v[indexes[j]]/x; 
  
            res = res * x; 
        } 
        else
            x++; 
    } 
  
    // Then multiply all reduced array elements 
    for (int i=0; i < v.size(); i++) 
        res = res*v[i]; 
  
    return res; 
} 

int main() {
    // key is step, value is vector of four states (for each moon)
    std::map<long long int, std::vector<Moon>> moons;
    // get initial position, for step = 0
    get_initial_state(moons[0]);

    std::vector<Moon> state = moons[0];
    for(int i = 0; i < 4; i++)
    std::printf("<x=%d, y=%d, z=%d>         <v_x=%d, v_y=%d, v_z=%d>\n",
        state[i]._x, state[i]._y, state[i]._z, state[i]._v_x, state[i]._v_y, state[i]._v_z);
    // calculate for defined number of steps
    for(long long int i = 1; i < NUMBER_OF_STEPS+1; i++) {
        int j = 0;
        state.clear();
        std::for_each(moons[i-1].begin(), moons[i-1].end(), [&](Moon moon) {
            // help contains all moons except moon to update
            std::vector<Moon> help = moons[i-1];
            help.erase(help.begin() + j);
            j++;

            moon.update_velocities(help);
            moon.update_positions();
            state.push_back(moon);

            // std::printf("<x=%d, y=%d, z=%d>         <v_x=%d, v_y=%d, v_z=%d>\n",
            // moon._x, moon._y, moon._z, moon._v_x, moon._v_y, moon._v_z);
        });

        moons[i] = state;
    }

    int part1 = calculate_total_energy(state);
    std::cout << "part1 = " << part1 << std::endl;

    // part2
    std::vector<int> periods;
    std::vector<long long int> x_values, y_values, z_values;
    std::vector<int> periods_flag(12, 0);
    int i = 1;
    moons.clear();
    get_initial_state(moons[0]);
    state = moons[0];

    while(periods.size() < 12) {
        int j = 0;
        state.clear();

        std::for_each(moons[i-1].begin(), moons[i-1].end(), [&](Moon moon) {
            // help contains all moons except moon to update
            std::vector<Moon> help = moons[i-1];
            help.erase(help.begin() + j);
            j++;

            moon.update_velocities(help);
            moon.update_positions();
            state.push_back(moon);

            // std::printf("<x=%d, y=%d, z=%d>         <v_x=%d, v_y=%d, v_z=%d>\n",
            // moon._x, moon._y, moon._z, moon._v_x, moon._v_y, moon._v_z);
        });

        moons[i] = state;
        std::cout << "korak = " << i << std::endl;
        std::cout << "periods size = " << periods.size() << std::endl;
        i++;

        for(int k = 0; k < 4; k++) {
            if(periods_flag[k * 3 + 0] == 1 && periods_flag[k * 3 + 1] == 1 &&
            periods_flag[k * 3 + 2] == 1) {
                continue;
            }
            for(int l = 0; l < moons.size(); l++) {
                // vector of all moons for i-th step
                std::vector<Moon> tmp = moons[l];
                
                x_values.push_back(tmp[k]._x);
                if(periods_flag[k*3+0] == 0 && period(x_values)) {
                    periods.push_back(x_values.size()/2);
                    periods_flag[k*3+0] = 1;
                }

                y_values.push_back(tmp[k]._y);
                if(periods_flag[k*3+1] == 0 && period(y_values)) {
                    periods.push_back(y_values.size()/2);
                    periods_flag[k*3+1] = 1;
                }

                z_values.push_back(tmp[k]._z);
                if(periods_flag[k*3+2] == 0 && period(z_values)) {
                    periods.push_back(z_values.size()/2);
                    periods_flag[k*3+2] = 1;
                }
            }
            x_values.clear();
            y_values.clear();
            z_values.clear();
        }
    }

    
    for(int i = 0; i < periods.size(); i++) {
        std::printf("period[%d] = %d\n", i, periods[i]);
    }

    std::cout << "period size = " << periods.size() << std::endl;

    std::cout << "part2 = " << lcm(periods) << std::endl;
}