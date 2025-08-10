// https://adventofcode.com/2020/day/24

#include "../aoc_lmap_utils.h"

//Tiles
//Coordinates

/*             (2,0) 
*         (1,0)
*    tile0     (0,1)     (0,2) 
*        (-1,1)     (-1,2)
* 
* e: (0,+1)
* w: (0,-1)
* ne: (+1,0)
* nw: (+1,-1)
* se: (-1,+1)
* sw: (-1,0)
* 
* */

// Rather than creating a grid, I will store changed tiles to black in a set.

bool TEST = false;
string test_file = "input_test.txt"; //9 tiles

void parse_input_lines(strings_t slines) {
    cout << "parse_input_lines()" << endl;

    return; //do nothing

}

struct CoordsHasher {
    size_t operator()(const pair<int, int>& p) const {
        // Combine hashes of x and y using the bitwise XOR

        //return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
        auto hash_y = hash<int>{}(p.first);
        auto hash_x = hash<int>{}(p.second);

        return hash_y ^ (hash_x << 1);

    }
};

unordered_set<pair <int, int>, CoordsHasher> black_tiles;

void part1(strings_t slines) {
    cout << "part1" << endl;


    for (const auto& s0 : slines){
        if (s0.size() > 0) {
            cout << "line : " << s0<<endl;

            pair <int, int> tile_loc = { 0,0 };
            
            for (auto it = s0.begin(); it != s0.end(); it++) {
                auto c = *it;
                
                if (c == 'n' || c == 's') {
                    it++;
                    auto c1 = *it; //check next character

                    if (c == 'n' && c1 == 'e') {
                        tile_loc.first++;
                    }
                    else if (c == 'n' && c1 == 'w') {
                        tile_loc.first++;
                        tile_loc.second--;
                    }
                    else if (c == 's' && c1 == 'e') {
                        tile_loc.first--;
                        tile_loc.second++;
                    }
                    else if (c == 's' && c1 == 'w') {
                        tile_loc.first--;
                    }
                }
                else if (c == 'e') {
                    tile_loc.second++;
                }
                else if (c == 'w') {
                    tile_loc.second--;
                }
            }
            cout << "coordinates " << tile_loc.first << " , " << tile_loc.second << endl;

            //Check if tile is already black
            if (black_tiles.find(tile_loc) == black_tiles.end()) {
                //Add item
                black_tiles.insert(tile_loc);
            }
            else {
                //turn white (remove)
                black_tiles.erase(tile_loc);
            }
        }
    }
    cout << "number of black tiles left " << black_tiles.size() << endl;

    return;
}


//Need function that check for number of adjacent black tiles
uint how_many_adjacent_black_at_coords(const decltype(black_tiles)& btiles, pair<int, int> yx) {
    //Check all directions for black tiles
    
    deque < pair<int, int>> relative_locs = {
        {0,1}, {0,-1},
        {1,0}, {1,-1},
        { -1,1 }, { -1,0 }
    };

    uint count_blacks = 0;

    for (const auto& rel0 : relative_locs) {
        decltype(yx) at_coord = { yx.first + rel0.first, yx.second+rel0.second };

        if (btiles.find(at_coord) != btiles.end()) {
            //found black tile
            count_blacks;
        }
    }

    return count_blacks;

}


void part2(int nrounds) {
    cout << "part2" << endl;

    decltype(black_tiles) black_tiles0(black_tiles);

    
    for (int i = 0; i < nrounds; i++) {
        cout << "Round " << i << endl;

        decltype(black_tiles) tiles_to_flip;

        decltype(black_tiles) white_tiles_adj_to_black;

        deque < pair<int, int>> relative_locs = {
            {0,1}, {0,-1},
            {1,0}, {1,-1},
            { -1,1 }, { -1,0 }
        };


        for (const auto& bl0 : black_tiles0) {

            // around white. Need to get the white tiles adjacent to each black
            uint adj_black_tiles_count = 0;
            for (const auto& rel0 : relative_locs) {
                decltype(bl0) at_coord = { bl0.first + rel0.first, bl0.second + rel0.second };

                if (black_tiles0.find(at_coord) == black_tiles0.end()) {
                    //found white tile
                    white_tiles_adj_to_black.insert(at_coord);
                }
                else {
                    //black tile
                    adj_black_tiles_count++;
                }
            }

            if (adj_black_tiles_count == 0 || adj_black_tiles_count > 2) {
                tiles_to_flip.insert(bl0);
            }
        }

        //Check around white tiles
        for (const auto& wh0 : white_tiles_adj_to_black) {

            // around white. Need to get the white tiles adjacent to each black
            uint adj_black_tiles_count = 0;
            for (const auto& rel0 : relative_locs) {
                decltype(wh0) at_coord = { wh0.first + rel0.first, wh0.second + rel0.second };

                if (black_tiles0.find(at_coord) != black_tiles0.end()) {
                    //black tile
                    adj_black_tiles_count++;
                }
            }

            if (adj_black_tiles_count == 2) {
                tiles_to_flip.insert(wh0);
            }
        }

        cout << "number of tiles to flip " << tiles_to_flip.size() << endl;

        for (const auto& tf0 : tiles_to_flip) {
            //Check if tile is already black
            if (black_tiles0.find(tf0) == black_tiles0.end()) {
                //tile is white, so flip it to black
                black_tiles0.insert(tf0);
            }
            else {
                //tile is black, turn to white (remove)
                black_tiles0.erase(tf0);
            }
        }

        cout << "round " << i << " complete . Number of black tires is " << black_tiles0.size() << endl;

    }
 }

int main() {
    //Read data from file to list of strings

    string input_file = "input.txt";
    if (TEST) {
        input_file = test_file;
    }
    auto slines = get_lines_from_file(input_file.c_str());

    //parse_input_lines(slines);

    part1(slines);


    //part 2: test 2208, 
    part2(100); //use object black_tiles (initial configuration)


    return 0;
}