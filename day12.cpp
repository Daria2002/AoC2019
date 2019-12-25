#include <map>
#include <vector>
#include <algorithm>

#define NUMBER_OF_STEPS 100

class Moon {

    public:
        Moon(int x, int y, int z) : _x(x), _y(y), _z(z)
        {
            _v_x = _v_y = _v_z = 0;
        }

        void update_velocities(std::vector<Moon> moon) {
            for(int i = 0; i < moon.size(); i++) {
                
            }
        }

        void update_positions(std::vector<Moon> moon) {

        }

    private:
        int _x, _y, _z, _v_x, _v_y, _v_z;
};

std::vector<Moon> get_initial_state() {

}

int main() {

    // key is step, value is vector of four states (for each moon)
    std::map<int, std::vector<Moon>> moons;

    // get initial position, for step = 0
    moons[0] = get_initial_state();

    std::vector<Moon> state;
    // calculate for defined number of steps
    for(int i = 1; i < NUMBER_OF_STEPS; i++) {
        // for every moon update position and velocity
        state = moons[i-1];
        std::for_each(state.begin(), state.end(), [&](Moon moon) {
            moon.update_velocities();
            moon.update_positions();
            state.push_back(moon);
        });
        moons[i] = state;
        state.empty();
    }

}