// https://adventofcode.com/2020/day/10

#include "../aoc_lmap_utils.h"

using namespace std;

void part1(vector<string> slines) {
    enum direction_t { EAST = 0, NORTH=1, WEST = 2, SOUTH=3}; //turn left is +1
    map< direction_t, pair<int, int> > dir_vectors = {
        {NORTH, {1,0} },
        {SOUTH, {-1,0} },
        {EAST, {0,1}},
        {WEST,{0,-1}}
    };

    vector<int> cur_pos = { 0,0 };
    direction_t cur_dir = EAST;

    for (const auto& line : slines) {
        cout << "instruction " << line << endl;
        //Get instruction letter
        char lett = line[0];

        // get number
        auto steps_s = line.substr(1, line.size() - 1);
        int steps = stoi(steps_s);

        //Execute instuction
        auto new_pos = cur_pos; //I'm almost certainly stl vector =operator copies

        bool do_transl = false;
        direction_t transl_dir = EAST;
        bool do_rotat = false;
        int rot_left = true;

        switch (lett) {
        case 'F':
            do_transl = true;
            transl_dir = cur_dir;
            break;
        case 'N':
            //Move north by the given value, don't change facing direction
            do_transl = true;
            transl_dir = NORTH;
            break;
        case 'S':
            do_transl = true;
            transl_dir = SOUTH;
            break;
        case 'E':
            do_transl = true;
            transl_dir = EAST;
            break;
        case 'W':
            do_transl = true;
            transl_dir = WEST;
            break;
        case 'L':
            // Rotate
            do_rotat = true;
            rot_left = true;
            break;
        case 'R':
            // Rotate
            do_rotat = true;
            rot_left = false;
            break;
        }


        if (do_transl) {
            cur_pos[0] += steps * dir_vectors[transl_dir].first;
            cur_pos[1] += steps * dir_vectors[transl_dir].second;
        }
        else if (do_rotat) {
            int rot_i = steps / 90;
            int rot_dir_mult = rot_left ? 1 : -1;

            auto new_dir = cur_dir + rot_dir_mult * rot_i;
            if (new_dir < 0) new_dir += 4;
            cur_dir = direction_t(new_dir%4);
            cout << "rotate, new direction " << cur_dir<<endl;
        }

        cout << "Position y,x " << cur_pos[0] << "," << cur_pos[1] << endl;
    }

    cout << "Manhattan distance " << abs(cur_pos[0]) + abs(cur_pos[1])<< endl;
}


void part2(vector<string> slines) {

    vector<long int> cur_pos = { 0,0 };
    vector<long int> wp_pos_rel = { 1,10 };

    //direction_t cur_dir = EAST;

    for (const auto& line : slines) {
        cout << "instruction " << line << endl;
        //Get instruction letter
        char lett = line[0];

        // get number
        auto steps_s = line.substr(1, line.size() - 1);
        int steps = stoi(steps_s);

        //Execute instuction
        auto new_pos = cur_pos; //default
        vector<long int> move_vector = { 0,0 };

        int nrot;
        switch (lett) {
        case 'F':

            new_pos[0] += steps * wp_pos_rel[0];
            new_pos[1] += steps * wp_pos_rel[1];

            cur_pos = new_pos;
            break;
        case 'N':
            //Move wp north by the given value
            wp_pos_rel[0] += steps;
            break;
        case 'S':
            wp_pos_rel[0] -= steps;
            break;
        case 'E':
            wp_pos_rel[1] += steps;
            break;
        case 'W':
            wp_pos_rel[1] -= steps;
            break;
        case 'L':
            // Rotate wp left
            nrot = steps / 90;
            for (int i = 0; i < nrot; i++) {
                wp_pos_rel = { wp_pos_rel[1], -wp_pos_rel[0] }; //rotation
            }

            break;
        case 'R':
            // Rotate right
            nrot = steps / 90;
            for (int i = 0; i < nrot; i++) {
                wp_pos_rel = { -wp_pos_rel[1], wp_pos_rel[0] }; //rotation
            }
            break;
        }

        cout << "Position y,x " << cur_pos[0] << "," << cur_pos[1] << "  Waypoint "<< wp_pos_rel[0]<<","<<wp_pos_rel[1] << endl;
    }

    cout << "Manhattan distance " << abs(cur_pos[0]) + abs(cur_pos[1]) << endl;
}



int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test.txt");
    auto slines = get_lines_from_file("input.txt");

    //part1(slines);
    part2(slines);

    return 0;
}