// https://adventofcode.com/2020/day/10

#include "../aoc_lmap_utils.h"

using namespace std;

const uint FLOOR = 0;
const uint EMPTY = 1;
const uint OCCUPIED = 2;


void part1(uint** seat_layout, uint nrows, uint ncols) {
    cout << "part1() , nrows,ncols=" << nrows<<","<<ncols<<endl;

    bool has_changed;
    uint count_iter = 0;

    auto last_seat_layout = seat_layout;

    //Create buffer for next iteration
    uint** new_seat_layout = new uint * [nrows];
    for (uint irow = 0; irow < nrows; irow++) {
        new_seat_layout[irow] = new uint[ncols];
        for (uint icol = 0; icol < ncols; icol++) {
            new_seat_layout[irow][icol] = 0; //floor by default
        }
    }

    //iterations
    do{
        has_changed = false;

        for (uint irow = 0; irow < nrows; irow++) {
            for (uint icol = 0; icol < ncols; icol++) {
                new_seat_layout[irow][icol] = last_seat_layout[irow][icol]; //default
                if (last_seat_layout[irow][icol] > 0) { //only count if it is a seat
                    

                    uint row_start = irow > 0 ? irow - 1  : irow;
                    uint row_end = irow < nrows - 1 ? irow + 1 : irow;
                    uint col_start = icol > 0 ? icol - 1  : icol;
                    uint col_end = icol < ncols - 1 ? icol + 1 : icol;

                    uint count_adj = 0;
                    for (uint r0 = row_start; r0 <= row_end; r0++) {
                        for (uint c0 = col_start; c0 <= col_end; c0++) { 
                            if (!(r0 == irow && c0 == icol)) { //don't consider this sit in the count
                                if (last_seat_layout[r0][c0] == OCCUPIED) {
                                    count_adj++;
                                }
                            }
                        }
                    }

                    //set new seat configuration
                    if (last_seat_layout[irow][icol] == EMPTY && count_adj == 0) {
                        //becomes occupied
                        new_seat_layout[irow][icol] = OCCUPIED;
                        has_changed = true;
                    }
                    else if (last_seat_layout[irow][icol] == OCCUPIED && count_adj >= 4) {
                        new_seat_layout[irow][icol] = EMPTY;
                        has_changed = true;
                    }
                }
            }
        }

        //Print seat layout

            //cout << "count_iter: " << count_iter << " , has_changed:" << has_changed << endl;
            //for (uint r0 = 0; r0 < nrows; r0++) {
            //    for (uint c0 = 0; c0 < ncols; c0++) {
            //        uint v = new_seat_layout[r0][c0];
            //        char c = '.';
            //        switch (v) {
            //        case 1:
            //            c = 'L';
            //            break;
            //        case 2:
            //            c = '#';
            //        }
            //        cout << c;
            //    }
            //    cout << endl;
            //}
            //cout << endl;


        //Swap pointers
        auto temp = new_seat_layout;
        new_seat_layout = last_seat_layout;
        last_seat_layout = temp;

        count_iter++;
    //}while (has_changed && count_iter<100);
    } while (has_changed);
    cout << "People stopped moving" << endl;

    //Get all occupied seats
    uint count_occ_seats=0;
    for (uint r0 = 0; r0 < nrows; r0++) {
        for (uint c0 = 0; c0 < ncols; c0++) {
            if (last_seat_layout[r0][c0] == OCCUPIED) count_occ_seats++;
        }
    }
    cout << "count_occ_seats: " << count_occ_seats << endl;
}


void part2(uint** seat_layout, uint nrows, uint ncols) {
    cout << "part2() , nrows,ncols=" << nrows << "," << ncols << endl;

    bool has_changed;
    uint count_iter = 0;

    auto last_seat_layout = seat_layout;

    //Create buffer for next iteration
    uint** new_seat_layout = new uint * [nrows];
    for (uint irow = 0; irow < nrows; irow++) {
        new_seat_layout[irow] = new uint[ncols];
        for (uint icol = 0; icol < ncols; icol++) {
            new_seat_layout[irow][icol] = 0; //floor by default
        }
    }

    vector<pair<int, int>> view_dirs = {
        {-1,0},{1,0},
        {0,-1},{0,1},
        {-1,-1},{-1,1},{1,-1},{1,1}
    };

    //iterations
    do {
        has_changed = false;

        for (uint irow = 0; irow < nrows; irow++) {
            for (uint icol = 0; icol < ncols; icol++) {
                uint cur_seat = last_seat_layout[irow][icol];

                new_seat_layout[irow][icol] = cur_seat; //default
                
                if (cur_seat > 0) { //only make changes if it is a seat

                    uint count_vis = 0;
                    
                    for (const auto& vdir : view_dirs) {
                        //Along all view directions
                        auto sr = vdir.first;
                        auto sc = vdir.second;

                        for (int r0=irow, c0=icol;
                            r0 >= 0 && r0 < nrows && c0 >= 0 && c0 < ncols;
                            r0 += sr, c0 += sc) {

                            if (!(r0 == irow && c0 == icol)) { //don't consider this sit in the count

                                if (last_seat_layout[r0][c0] == OCCUPIED) {
                                    count_vis++;
                                    break;
                                }
                                else if (last_seat_layout[r0][c0] == EMPTY) {
                                    break;
                                }
                            }

                        }
                    }

                    //set new seat configuration
                    if (cur_seat == EMPTY && count_vis == 0) {
                        //becomes occupied
                        new_seat_layout[irow][icol] = OCCUPIED;
                        has_changed = true;
                    }
                    else if (cur_seat == OCCUPIED && count_vis >= 5) { //five or more, tolerant
                        new_seat_layout[irow][icol] = EMPTY;
                        has_changed = true;
                    }
                }
            }
        }

        //Print seat layout

        //cout << "count_iter: " << count_iter << " , has_changed:" << has_changed << endl;
        //for (uint r0 = 0; r0 < nrows; r0++) {
        //    for (uint c0 = 0; c0 < ncols; c0++) {
        //        uint v = new_seat_layout[r0][c0];
        //        char c = '.';
        //        switch (v) {
        //        case 1:
        //            c = 'L';
        //            break;
        //        case 2:
        //            c = '#';
        //        }
        //        cout << c;
        //    }
        //    cout << endl;
        //}
        //cout << endl;


        //Swap pointers
        auto temp = new_seat_layout;
        new_seat_layout = last_seat_layout;
        last_seat_layout = temp;

        count_iter++;
        //}while (has_changed && count_iter<100);
    } while (has_changed);

    cout << "People stopped moving" << endl;

    //Get all occupied seats
    uint count_occ_seats = 0;
    for (uint r0 = 0; r0 < nrows; r0++) {
        for (uint c0 = 0; c0 < ncols; c0++) {
            if (last_seat_layout[r0][c0] == OCCUPIED) count_occ_seats++;
        }
    }
    cout << "count_occ_seats: " << count_occ_seats << endl;
}


int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test.txt");
    auto slines = get_lines_from_file("input.txt");

    //Assume that the number or lines is the number of rows of the array
    //uint** seats = create2DArray<uint>(slines.size(), slines[0].size());

    uint nrows = slines.size();
    uint ncols = slines[0].size();

    uint** seat_layout = new uint*[nrows];
    for (uint irow = 0; irow < nrows; irow++) {
        seat_layout[irow] = new uint[ncols];
        auto line = slines[irow];
        for (uint icol = 0; icol < ncols; icol++) {
            seat_layout[irow][icol] = FLOOR; //floor by default
            if (line[icol] == 'L') {
                seat_layout[irow][icol] = EMPTY; //1:empty, 2:occupied 
            }
        }
    }

    //part1(seat_layout, nrows, ncols);
    part2(seat_layout, nrows, ncols);

    //delete
    for (uint irow = 0; irow < nrows; irow++) {
        delete[] seat_layout[irow];
    }
    delete[] seat_layout;

    return 0;
}