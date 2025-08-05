// https://adventofcode.com/2020/day/20

// This solution here is far from optimized but it gives the right result
// 
//part 2 solution 1629

#include "../aoc_lmap_utils.h"

bool TEST = false;
string test_file = "input_test.txt"; //9 tiles

using type_img_bool = vector<vector<bool>>;

class cTile
{
public:
    type_img_bool m_tile_bool;
    //type_img_bool tile_flip;
    uint m_tile_id = 0;
    bool m_is_tile = false;

    uint m_count_rots = 0;
    bool m_is_flipped = false;

    cTile() {
    }

    cTile (const type_img_bool& tile_bool0, uint tile_id, uint count_rots=0, bool is_flipped=false) {
        m_tile_bool = type_img_bool(tile_bool0);
        m_tile_id = tile_id;
        m_is_tile = true;
        m_count_rots = count_rots;
        m_is_flipped = is_flipped;
    }

    static cTile from_slines(const strings_t& slines, uint tile_id) {
        // for each line, convert '.' to false, and '#' to 1
        type_img_bool tile_bool0;

        for (const auto& s : slines) {
            if (s.size() > 0) {
                vector<bool> img_row;
                for (const auto& c : s) {
                    if (c == '.') {
                        img_row.push_back(false);
                    }
                    else if (c == '#') {
                        img_row.push_back(true);
                    }
                }
                tile_bool0.push_back(img_row);
            }
        }
        cTile tile0(tile_bool0, tile_id);

        return tile0;

    };

    cTile flip_ud() {
        //Flip tile up-down
        if (!m_is_tile)  cerr << "Not a valid tile" << endl;
        
        //reverse by using the range constructor
        //type_img_bool reversed(m_tile_bool.end(), m_tile_bool.begin());//crashing
        type_img_bool reversed(m_tile_bool.rbegin(), m_tile_bool.rend());

        cTile rev_tile(reversed, m_tile_id, m_count_rots, !m_is_flipped);
        return rev_tile;
    }

    cTile rotate_ac() {
        if (!m_is_tile)  cerr << "Not a valid tile" << endl;

        uint h = m_tile_bool.size();
        uint w = m_tile_bool[0].size();

        uint new_w = h;
        uint new_h = w;

        type_img_bool new_im;

        for (int ih = 0; ih < new_h; ih++) {
            vector<bool> new_row(new_w, false); //initialise row with false
            for (int iw = 0; iw < new_w; iw++) {
                new_row[iw] = m_tile_bool[iw][w - ih - 1];
            }
            new_im.push_back(new_row);
        }

        cTile newTile(new_im, m_tile_id, m_count_rots+1, m_is_flipped);
        return newTile;
    }

    vector<uint> get_sides_in_dec() {
        if (!m_is_tile)  cerr << "Not a valid tile" << endl;
        //convert the pixels on the sides to binary and return 4 decimals
        // decimals are read clockwise.
        uint top(0), right(0), bottom(0), left(0); //initialise with zeros

        //top
        auto vb = m_tile_bool[0];
        for (const bool& b : vb) {
            if (b) {
                top += 1;
            }
            top = top << 1; //shift
        }

        const uint h = m_tile_bool.size();

        // left and right
        for (uint i = 0; i < h; i++) {
            bool b;

            //right. rows from top to bottom (clockwise)
            b = m_tile_bool[i].back();
            if (b) {
                right += 1;
            }
            right = right << 1;

            //left: rows from bottom to top
            b = m_tile_bool[h - i - 1].front();
            if (b) {
                left += 1;
            }
            left = left << 1;
        }

        //bottom
        vb = m_tile_bool.back();
        for (int i = vb.size() - 1; i >= 0; i--) {
            bool b = vb[i];
            if (b) {
                bottom += 1;
            }
            bottom = bottom << 1; //shift
        }

        return vector<uint> {top, right, bottom, left};

    }

    void print(bool inc_title=true) {
        //print tile to console

        if (!m_is_tile) {
            cout << "Empty tile" << endl;
            return;
        }

        if (inc_title) {
            cout << "Tile " << m_tile_id << endl;
        }
        for (const auto& tilerow : m_tile_bool) {
            for (auto pix_value : tilerow) {
                char c = '.';
                if (pix_value) c = '#';
                cout << c;
            }
            cout << endl;
        }
    }

};



deque<cTile> all_raw_tiles;
unordered_map <uint, cTile> map_id_to_tile;

void get_coords_from_next_ipos(uint next_ipos, uint& ix, uint& iy) {
    //assumes board is square

    uint pseudo_radius = (uint)floor(sqrt(next_ipos));

    uint pseudo_angle = 0;
    if (pseudo_radius > 0) {
        pseudo_angle = next_ipos - (pseudo_radius * pseudo_radius);
    }

    cout << "next_ipos: " << next_ipos << " , pseudo_radius: " << pseudo_radius << " , pseudo_angle: " << pseudo_angle << ", ";

    if (pseudo_angle <= pseudo_radius) {
        ix = pseudo_radius;
        iy = pseudo_angle;
    }
    else {
        ix = 2 * pseudo_radius - pseudo_angle;
        iy = pseudo_radius;
    }

    cout << "ix: " << ix << " , iy: " << iy << endl;

    //Tested, OK
}


bool check_can_place_tile_at_pos(vector<vector<cTile>>& current_board, cTile& tile0, uint ix, uint iy) {
    //Return true if tile can be placed
    //does not check if tile is available
    //Does not update current_board
    //does not check rotations.

    auto tile0_decs = tile0.get_sides_in_dec();

    //check up
    if (iy >= 1) {
        cTile tile_up = current_board[iy - 1][ix]; // the important side will be up
        if (tile_up.m_is_tile) {
            tile_up = tile_up.flip_ud();
            auto tile_up_decs = tile_up.get_sides_in_dec();

            if (tile_up_decs[0] != tile0_decs[0]) {
                return false;
            }
        }
    }
    //check down
    if (iy < current_board.size()-1) {
        cTile tile_down = current_board[iy+1][ix]; // the important side will be down
        if (tile_down.m_is_tile) {
            tile_down = tile_down.flip_ud();
            auto tile_down_decs = tile_down.get_sides_in_dec();
            if (tile_down_decs[2] != tile0_decs[2]) {
                return false;
            }
        }
    }

    //check right
    if (ix < current_board[0].size() - 1) {
        cTile tile_right = current_board[iy][ix + 1];
        if (tile_right.m_is_tile) {
            tile_right= tile_right.flip_ud(); // the important side will be left
            auto tile_right_decs = tile_right.get_sides_in_dec();
            if (tile_right_decs[3] != tile0_decs[1]) {
                return false;
            }
        }
    }
    //check left
    if (ix >= 1 ) {
        cTile tile_left = current_board[iy][ix-1]; // the important side will be right
        if (tile_left.m_is_tile) {
            tile_left = tile_left.flip_ud();
            auto tile_left_decs = tile_left.get_sides_in_dec();
            if (tile_left_decs[1] != tile0_decs[3]) {
                return false;
            }
        }
    }

    cout << "Can place tile " << tile0.m_tile_id << " at location y,x " << iy << "," << ix << endl;
    return true;
}

vector<vector<cTile>> final_board;


bool part1_dfs(vector<vector<cTile>> current_board, const set<uint> remaining_tiles_ids, const uint next_ipos) {
    cout << "** part1_dfs ** " << next_ipos << ", remaining ids count:"<< remaining_tiles_ids.size()<< endl;
    // get board size
    uint board_h = current_board.size();
    uint board_w = current_board[0].size();

    //Check if need to look for more
    if (remaining_tiles_ids.size() <= 0) {
        cout << "No more remaining tiles. Board ok." << endl;
        final_board = current_board;
        return true;
    }
    if (next_ipos >= board_h * board_w) {
        cout << "No more remaining positions to explore. Board ok." << endl;
        final_board = current_board;
        return true;
    }

    uint ix = 0;
    uint iy = 0;

    get_coords_from_next_ipos(next_ipos, ix, iy); //this function returns ix and iy

    //loop through available tiles, flip and rotations and check all directions if possible
    for (const auto& id0: remaining_tiles_ids) {
        auto cTile0 = map_id_to_tile[id0];

        //flip
        for (auto iflip0 : { 0,1 }) {
            
            cTile cTileRot(cTile0);//clone
            for (auto irot : { 0,1,2,3 }) {

                bool b = check_can_place_tile_at_pos(current_board, cTileRot, ix, iy);

                if (b) {
                    //place tile
                    vector <vector<cTile>> current_board1(current_board);//clone
                    set<uint> remaining_tiles_ids1(remaining_tiles_ids); //clone

                    current_board1[iy][ix] = cTileRot;
                    remaining_tiles_ids1.erase(cTileRot.m_tile_id);

                    auto next_ipos1 = next_ipos + 1;

                    bool res = part1_dfs(current_board1, remaining_tiles_ids1, next_ipos1);
                    if (res) {
                        return true;
                    }
                }

                cTileRot = cTileRot.rotate_ac();
            }


            cTile0 = cTile0.flip_ud();
        }



    }

    cout << "No tiles found for next_ipos: " << next_ipos << endl;
    return false; //TODO

};


void part1() {
    cout << "part1" << endl;

    /*
    * Plan
    * Use all_tiles_map,
    * Create a set to keep track of the tiles used, to be used.
    *
    * Collect a count of decimal values of the sides (unordered_map)
    * Do this for the tile and its flipped version
    *
    * Find 4 tiles tha have non-matching sides.
    * Also, they should perhaps have adjacent non-matching sides
    *
    */

    cout << "Generating flipped versions" << endl;

    deque<cTile> tiles_incl_flipped(all_raw_tiles);

    unordered_map<uint, uint> count_dec_sides;

    for (auto& tile0 : all_raw_tiles) {
        auto tile_flip = tile0.flip_ud();

        tiles_incl_flipped.push_back(tile_flip);

        //Also count occurences of decimal sides
        for (auto i : tile0.get_sides_in_dec()) {
            count_dec_sides[i]++;
        }
        for (auto i : tile_flip.get_sides_in_dec()) {
            count_dec_sides[i]++;
        }

    }

    cout << "Count of decimals on sides of tiles including the flipped versions" << endl;
    for (auto pair0 : count_dec_sides) {
        cout << pair0.first << " : " << pair0.second << endl;
    }

    //Corner candidates
    // Two adjacent sides with no match, and two other sides with matches.
    cout << "Finding corner candidates" << endl;
    //Go through the tiles and check for adjacent numbers
    deque<cTile> corner_candidates;
    deque<uint> corner_candidates_i;

    for (auto tile_i : tiles_incl_flipped) {
        auto dec_numbers = tile_i.get_sides_in_dec();

        for (int i = 0; i < 4; i++) {
            uint dec0 = dec_numbers[i];
            uint j = (i + 1) % 4;

            uint dec1 = dec_numbers[i];

            if (count_dec_sides[dec0] == 1 && count_dec_sides[dec1] == 1) {
                //two adjacent non-matching 
                //check the other two
                uint i0 = (i + 2) % 4;
                uint j0 = (j + 2) % 4;
                if (count_dec_sides[dec_numbers[i0]] > 1 && count_dec_sides[dec_numbers[j0]] > 1) {
                    corner_candidates.push_back(tile_i);
                    corner_candidates_i.push_back(i);
                }
            }
        }
    }
    if (corner_candidates.size() > 0) {

        cout << "Corner candidates. size:" << corner_candidates.size() << endl;
        for (const auto& tile_i : corner_candidates) {
            cout << tile_i.m_tile_id << " , ";
        }
        cout << endl;


        uint tiles_width = floor(sqrt(all_raw_tiles.size()));
        cout << "tiles_width: " << tiles_width << endl;


        set<uint> remaining_tiles_ids;  //ordered, starting with the corner candidates
        for (const auto& tile0 : all_raw_tiles) {
            remaining_tiles_ids.insert(tile0.m_tile_id);
        }

        //start with the corner candidates
        for (int i = 0; i < corner_candidates.size(); i++) {

            //Setup initial board
            vector<vector<cTile>> board_start(tiles_width, vector<cTile>(tiles_width));

            auto corner_candidate0 = corner_candidates[i];
            cout << "Starting with tile " << corner_candidate0.m_tile_id << endl;

            auto irot = corner_candidates_i[i];

            auto corner_candidate_rot = corner_candidate0.rotate_ac();
            for (int i = 0; i < irot ; i++) {
                corner_candidate_rot = corner_candidate_rot.rotate_ac();
            }

            board_start[0][0] = corner_candidate_rot;
            auto remaining_tiles_ids0(remaining_tiles_ids); //clone
            remaining_tiles_ids0.erase(corner_candidate_rot.m_tile_id);

            cout << "Begin DFS" << endl;

            if (part1_dfs(board_start, remaining_tiles_ids0, 1)) {
                cout << "SOLUTION FOUND" << endl;
                //print board ids
                
                
                for (int iy = 0; iy < final_board.size(); iy++) {
                    auto board_row = final_board[iy];
                    for (int ix = 0; ix < board_row.size(); ix++) {
                        auto tile0 = board_row[ix];
                        cout << tile0.m_tile_id << "f" << tile0.m_is_flipped << "r"<< tile0.m_count_rots << " ";
                    }
                    cout << endl;
                }

                cout << endl;
                
                
                uintll corners_mult = final_board[0][0].m_tile_id;
                corners_mult *= final_board[0].back().m_tile_id;
                corners_mult *= final_board.back()[0].m_tile_id;
                corners_mult *= final_board.back().back().m_tile_id;
                cout << "corners mult : " << corners_mult << endl;

                return;
            }

        }

    }
    else {
        cout << "No corner candidates found" << endl;
    }

}

/* Solution ids, calculated in part1

3187  2861  3137  3643  1019  2293  2423  2767  2347  2179  1499  2239
2251  1889  3461  2731  1787  3943  3407  2689  1637  1913  1697  3271
1423  2039  1171  1951  2789  3121  2797  1657  3593  3547  3191  1877
3533  1733  1567  1181  2137  3011  2213  2647  2417  1021  3061  1277
2029  3833  3517  2089  2113  3559  3539  3019  1583  2267  3659  3613
1559  1667  1283  3413  1307  3253  3931  1523  3229  1039  2909  1091
1427  2531  3389  1129  3767  2017  2069  2957  2207  1327  2687  3491
1109  1847  1811  1093  2677  2927  2357  1259  1097  2711  3677  2659
3803  1051  1741  1543  2999  1871  1237  2857  3877  1721  1453  2521
2011  2879  3221  2099  1823  1223  2791  1709  1319  3463  3761  2843
2833  3923  2377  3083  3581  2053  2953  1627  1663  1933  3709  2837
3851  2339  2297  1861  2341  1607  2351  3433  3449  3391  3631  2503


3187f0r4 2861f1r3 3137f1r3 3643f1r2 1019f0r3 2293f0r3 2423f0r2 2767f0r2 2347f1r0 2179f1r2 1499f1r3 2239f0r1
2251f1r2 1889f1r0 3461f0r2 2731f1r2 1787f0r0 3943f0r3 3407f1r0 2689f1r3 1637f0r0 1913f0r1 1697f0r1 3271f0r3
1423f0r1 2039f0r3 1171f0r3 1951f0r2 2789f1r3 3121f1r2 2797f0r1 1657f1r2 3593f1r0 3547f1r0 3191f1r2 1877f0r1
3533f0r2 1733f0r2 1567f1r0 1181f0r2 2137f1r0 3011f0r3 2213f0r3 2647f0r3 2417f0r2 1021f0r0 3061f1r2 1277f0r0
2029f0r1 3833f1r0 3517f0r0 2089f1r3 2113f1r2 3559f0r3 3539f1r0 3019f0r1 1583f1r0 2267f1r0 3659f1r3 3613f1r3
1559f0r1 1667f0r1 1283f1r3 3413f1r0 1307f1r0 3253f0r1 3931f0r2 1523f1r3 3229f0r2 1039f1r2 2909f0r0 1091f1r0
1427f1r3 2531f0r1 3389f0r3 1129f1r3 3767f1r3 2017f1r3 2069f1r2 2957f1r0 2207f1r0 1327f1r0 2687f0r1 3491f1r3
1109f1r2 1847f0r1 1811f1r3 1093f0r0 2677f0r2 2927f1r0 2357f0r2 1259f1r2 1097f0r2 2711f0r2 3677f1r3 2659f1r2
3803f0r3 1051f1r0 1741f0r0 1543f1r2 2999f0r0 1871f1r2 1237f1r0 2857f1r0 3877f1r3 1721f1r0 1453f1r3 2521f0r0
2011f1r3 2879f0r3 3221f0r1 2099f0r3 1823f1r2 1223f0r1 2791f1r0 1709f0r0 1319f0r2 3463f1r2 3761f1r2 2843f1r0
2833f0r1 3923f1r2 2377f0r3 3083f0r2 3581f0r0 2053f0r2 2953f0r1 1627f1r3 1663f1r0 1933f1r3 3709f0r3 2837f1r0
3851f1r0 2339f0r2 2297f1r0 1861f0r0 2341f1r0 1607f1r2 2351f0r3 3433f1r3 3449f0r2 3391f0r3 3631f0r2 2503f0r1

f for flip,
r for rotation count

*/

string part1_res =
R"""(3187f0r4 2861f1r3 3137f1r3 3643f1r2 1019f0r3 2293f0r3 2423f0r2 2767f0r2 2347f1r0 2179f1r2 1499f1r3 2239f0r1
2251f1r2 1889f1r0 3461f0r2 2731f1r2 1787f0r0 3943f0r3 3407f1r0 2689f1r3 1637f0r0 1913f0r1 1697f0r1 3271f0r3
1423f0r1 2039f0r3 1171f0r3 1951f0r2 2789f1r3 3121f1r2 2797f0r1 1657f1r2 3593f1r0 3547f1r0 3191f1r2 1877f0r1
3533f0r2 1733f0r2 1567f1r0 1181f0r2 2137f1r0 3011f0r3 2213f0r3 2647f0r3 2417f0r2 1021f0r0 3061f1r2 1277f0r0
2029f0r1 3833f1r0 3517f0r0 2089f1r3 2113f1r2 3559f0r3 3539f1r0 3019f0r1 1583f1r0 2267f1r0 3659f1r3 3613f1r3
1559f0r1 1667f0r1 1283f1r3 3413f1r0 1307f1r0 3253f0r1 3931f0r2 1523f1r3 3229f0r2 1039f1r2 2909f0r0 1091f1r0
1427f1r3 2531f0r1 3389f0r3 1129f1r3 3767f1r3 2017f1r3 2069f1r2 2957f1r0 2207f1r0 1327f1r0 2687f0r1 3491f1r3
1109f1r2 1847f0r1 1811f1r3 1093f0r0 2677f0r2 2927f1r0 2357f0r2 1259f1r2 1097f0r2 2711f0r2 3677f1r3 2659f1r2
3803f0r3 1051f1r0 1741f0r0 1543f1r2 2999f0r0 1871f1r2 1237f1r0 2857f1r0 3877f1r3 1721f1r0 1453f1r3 2521f0r0
2011f1r3 2879f0r3 3221f0r1 2099f0r3 1823f1r2 1223f0r1 2791f1r0 1709f0r0 1319f0r2 3463f1r2 3761f1r2 2843f1r0
2833f0r1 3923f1r2 2377f0r3 3083f0r2 3581f0r0 2053f0r2 2953f0r1 1627f1r3 1663f1r0 1933f1r3 3709f0r3 2837f1r0
3851f1r0 2339f0r2 2297f1r0 1861f0r0 2341f1r0 1607f1r2 2351f0r3 3433f1r3 3449f0r2 3391f0r3 3631f0r2 2503f0r1
)""";

string sea_monster=
R"""(                  # 
#    ##    ##    ###
 #  #  #  #  #  #   
)""";

void part2() {

    //parse this to create an image that matches description of problem part2
    vector<vector<bool>> res_im;

    // Remove borders on all of tiles

    if (final_board.size() == 0) {
        //in case part1 was skipped
        //parse the string defined above part1_res to regenerate the board

        //not implemented
    }
    else {
        //use final_board
        //Get img sizes
        uint tiles_row_count = final_board.size();
        uint tiles_col_count = final_board[0].size();

        //each image
        uint tile_h = final_board[0][0].m_tile_bool.size();
        uint tile_w = final_board[0][0].m_tile_bool[0].size();

        uint res_h = (tile_h - 2) * tiles_row_count;
        uint res_w = (tile_w - 2) * tiles_col_count;

        

        for (int irow = 0; irow < res_h; irow++) {
            auto dvr = div(irow, (tile_h - 2));
            auto tile_r = dvr.quot;
            auto irow0 = dvr.rem+1;

            vector<bool> res_row(res_w);

            for (int icol = 0; icol < res_w; icol++) {
                auto dvc = div(icol, (tile_w - 2));
                auto tile_c = dvc.quot;
                auto icol0 = dvc.rem+1;

                auto tile0 = final_board[tile_r][tile_c];
                res_row[icol] = tile0.m_tile_bool[irow0][icol0];
            }
            res_im.push_back(res_row);
        }
        cout << "Stitched image created with size height,width " << res_im.size() << "," << res_im[0].size() << endl;

        //Show image
        for (const auto& res_row : res_im) {
            for (const auto& res_col : res_row) {
                if (res_col) {
                    cout << '#';
                }
                else {
                    cout << '.';
                }
            }
            cout << endl;
        }
    }   

    //parse sea monster
    stringstream ss(sea_monster);
    string stemp;
    deque<pair<uint,uint>> monster_marks_locs;

    uint irow = 0;
    while (std::getline(ss, stemp, '\n')) {
        for (int icol = 0; icol < stemp.size(); icol++) {
            if (stemp[icol] == '#') {
                monster_marks_locs.push_back({ irow,icol });
            }
        }
        irow++;

        cout << stemp << endl;
    }

    //monster represented in location where marks are

    //Find monster
    //As I may need to flip and rotate to find monsters
    //Convert it to tile, as these operations are already there
    cTile cTile0(res_im, 0, 0, false);
    deque < pair<uint, uint>> monster_locations;

    //flip
    for (auto iflip0 : { 0,1 }) {
        cTile cTileRot(cTile0);//clone
        for (auto irot : { 0,1,2,3 }) {

            auto res_im_bytes = cTileRot.m_tile_bool;

            uint res_h = res_im_bytes.size();
            uint res_w = res_im_bytes[0].size();

            for (int loc_row = 0; loc_row < res_im_bytes.size(); loc_row++) {
                for (int loc_col = 0; loc_col < res_im_bytes[0].size(); loc_col++) {
                    int is_monster = -1; //defaults to -1 for unknown, 0 for false
                    for (const auto& yx : monster_marks_locs) {

                        uint looky = loc_row + yx.first;
                        if (looky >= res_h) {
                            is_monster = 0;
                            break;
                        }
                        uint lookx = loc_col + yx.second;
                        if (lookx >= res_w) {
                            is_monster = 0;
                            break;
                        }
                        if (!res_im_bytes[looky][lookx]) {
                            is_monster = 0;
                            break;
                        }
                        else {
                            is_monster = 1; //maybe
                        }
                    }
                    if (is_monster == 1) {
                        cout << "Monster found at location " << loc_row << ", " << loc_col << endl;
                        monster_locations.push_back({ loc_row,loc_col });

                    };
                }
            }

            if (monster_locations.size() > 0) {
                cout << "Enough monsters. Lets calculate." << endl;

                type_img_bool res_im_bytes1(res_im_bytes); //clones

                for (auto mloc : monster_locations) {

                    for (const auto& yx : monster_marks_locs) {

                        uint looky = mloc.first + yx.first;
                        uint lookx = mloc.second + yx.second;

                        res_im_bytes1[looky][lookx] = false;
                    }
                
                }

                //Count hashes left
                uint count_hashes = 0;
                for (const auto& imrow : res_im_bytes1) {
                    for (const auto& imcol : imrow) {
                        if (imcol) {
                            count_hashes++;
                        }
                    }
                }
                cout << "Number of hashes left " << count_hashes << endl;

                break;//leave
            }


            cTileRot = cTileRot.rotate_ac();
        }

        if (monster_locations.size() > 0) {
            break;
        }

        cTile0 = cTile0.flip_ud();
    }
}

int main() {
    //Read data from file to list of strings

    string input_file = "input.txt";
    if (TEST) {
        input_file = test_file;
    }
    auto slines = get_lines_from_file(input_file.c_str());


    //Parse text to image objects

    int tilenumber = -1;
    strings_t s_tile_lines;
    uint count_tiles = 0;

    for (const auto& s : slines) {

        int pos0 = s.find("Tile", 0);
        if (pos0 != s.npos) {
            //new tile, get the number
            tilenumber = stoi(s.substr(5, s.size() - 5));

            s_tile_lines.clear();

        }
        else if (s != "") {

            s_tile_lines.push_back(s);
        }
        else {
            // s==""
            //Tile end, process

            auto newTile = cTile::from_slines(s_tile_lines, tilenumber);
            all_raw_tiles.push_back(newTile);
            count_tiles += 1;

            //reset
            s_tile_lines.clear();
            tilenumber = -1;
        }
    }
    //Check if there is a last tile to be processed

    if (s_tile_lines.size() > 0) {
        auto newTile = cTile::from_slines(s_tile_lines, tilenumber);
        all_raw_tiles.push_back(newTile);
        count_tiles += 1;
    }

    //Maps all raw tiles
    for (const auto& ctile0 : all_raw_tiles) {
        map_id_to_tile[ctile0.m_tile_id] = ctile0;
    }

    cout << "Number of tiles: " << all_raw_tiles.size() << endl;


    //Debug get_coords_from_next_ipos
     //Tested ok
    //uint ix = 0;
    //uint iy = 0;
    //for (int i0 = 0; i0 < 16; i0++) {
    //    get_coords_from_next_ipos(i0, ix,iy);
    //}

    part1();


    //Use last solution to work out part2
    part2();


    return 0;
}