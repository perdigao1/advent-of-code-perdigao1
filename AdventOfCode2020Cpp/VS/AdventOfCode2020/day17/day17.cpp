// https://adventofcode.com/2020/day/17

#include "../aoc_lmap_utils.h"
#include <unordered_set>
#include <cassert>

using namespace std;


//auto create_bool_3D_array(uint nz, uint ny, uint nx) {
    ////Create array in a safe matter

    //// Allocate memory for the 3D array using unique_ptr
    //auto my3DArray = std::make_unique<std::unique_ptr<std::unique_ptr<bool[]>[]>[]>(nx);
    //for (size_t i = 0; i < nx; ++i) {
    //    my3DArray[i] = std::make_unique<std::unique_ptr<bool[]>[]>(ny);
    //    for (size_t j = 0; j < ny; ++j) {
    //        my3DArray[i][j] = std::make_unique<bool[]>(nz);
    //    }
    //}
    ////fill with false
    //for (uint i = 0; i < nx; ++i) {
    //    for (uint j = 0; j < ny; ++j) {
    //        for (uint k = 0; k < nz; ++k) {
    //            my3DArray[i][j][k] = false;
    //        }
    //    }
    //}

    //std::vector<std::vector<std::vector<bool>>> my3DArray(nx, std::vector<std::vector<bool>>(ny, std::vector<bool>(nz)));

template <typename T>
T*** create_3D_array(uint nz, uint ny, uint nx, T fill) {

    T*** my3DArray = new T * *[nz];
    for (uint iz = 0; iz < nz; iz++) {
        my3DArray[iz] = new T * [ny];
        for (uint iy = 0; iy < ny; iy++) {
            my3DArray[iz][iy] = new T[nx];
            for (uint ix = 0; ix < nx; ix++) {
                my3DArray[iz][iy][ix] = fill;
            }
        }
    }

    return my3DArray;
};


bool DEBUG = true;

void print_bool_array(bool*** barr, uint nz,uint ny,uint nx) {
    if (DEBUG) {
        for (uint iz = 0; iz < nz; iz++) {
            cout << "slice : " << iz << endl;
            for (uint iy = 0; iy < ny; iy++) {
                for (uint ix = 0; ix < nx; ix++) {
                    if (barr[iz][iy][ix]) {
                        cout << "#";
                    }
                    else {
                        cout << ".";
                    }
                }
                cout << endl;
            }
        }
    }
}


void part1(vector<string> slines) {
    //Get initial layout of grid as array of bools

    uint i_nx = slines[0].size();
    uint i_ny = slines.size();
    
    //Create an array that is larger by 6 on each direction
    const uint NCYCLES = 6;

    uint nz = 1 + 2 * NCYCLES;
    uint ny = i_ny + 2 * NCYCLES;
    uint nx = i_nx + 2 * NCYCLES;

    auto my3DArray = create_3D_array<bool>(nz, ny, nx, false);

    // Initialize the array with init values
    uint iz = 0;
    
    for (uint iy = 0; iy < i_ny; iy++) {
        auto s0 = slines[iy];
        for (uint ix = 0; ix < i_nx; ix++) {
            auto c = s0[ix];
            if (c == '#') {
                my3DArray[iz+NCYCLES][iy+NCYCLES][ix+NCYCLES] = true;
            }
        }
    }
    
    cout << "Initialization complete" << endl;

    //Counting neighbors

    /*
    * 
    Each cube only ever considers its neighbors:
    any of the 26 other cubes where any of their coordinates differ by at most 1. 
    For example, given the cube at x=1,y=2,z=3,
    its neighbors include the cube at x=2,y=2,z=2, the cube at x=0,y=2,z=3, and so on.
    * 
    * */


    // This below is wrongly implemented
    // Need to count diagonal ones too

    auto buf_arr = create_3D_array<bool>(nz, ny, nx, false);
    
    auto count_neighb = create_3D_array<uint>(nz, ny, nx, 0);

    for (uint icycle = 0; icycle < NCYCLES; icycle++) {
        
        cout << "cycle : " << icycle << endl;
        print_bool_array(my3DArray, nz,ny,nx);

        for (uint iz = 0; iz < nz; iz++) {

            uint iz0 = 0; if (iz > 0) iz0 = iz-1;
            uint iz1 = nz - 1; if (iz < nz - 1) iz1 = iz+1;

            for (uint iy = 0; iy < ny; iy++) {
                uint iy0 = 0; if (iy > 0) iy0 = iy - 1;
                uint iy1 = ny - 1; if (iy < ny - 1) iy1 = iy + 1;

                for (uint ix = 0; ix < nx; ix++) {
                    uint ix0 = 0; if (ix > 0) ix0 = ix - 1;
                    uint ix1 = nx - 1; if (ix < nx - 1) ix1 = ix + 1;

                    uint c = 0;
                    //Check all directions

                    for (auto iz2 = iz0; iz2 <= iz1; iz2++) {
                        for (auto iy2 = iy0; iy2 <= iy1; iy2++) {
                            for (auto ix2 = ix0; ix2 <= ix1; ix2++) {
                                if ((iz2 != iz || iy2 != iy || ix2 != ix) &&
                                    my3DArray[iz2][iy2][ix2]) {
                                    c++;
                                }
                            }
                        }
                    }
                    assert(c <= 26);

                    count_neighb[iz][iy][ix] = c;
                }
            }
        }
        for (uint iz = 0; iz < nz; iz++) {
            for (uint iy = 0; iy < ny; iy++) {
                for (uint ix = 0; ix < nx; ix++) {
                    //Adjust based on value
                    auto c0 = count_neighb[iz][iy][ix];
                    if (my3DArray[iz][iy][ix]) {
                        if (c0 == 2 || c0 == 3) {
                            buf_arr[iz][iy][ix] = true;
                        }
                        else {
                            buf_arr[iz][iy][ix] = false;
                        }
                    }
                    else {
                        if (c0 == 3) {
                            buf_arr[iz][iy][ix] = true;
                        }
                        else {
                            buf_arr[iz][iy][ix] = false;
                        }
                    }
                }
            }
        }

        //new cycle
        swap(my3DArray, buf_arr);
    }

    cout << "final cycle : " << endl;
    print_bool_array(my3DArray, nz, ny, nx);

    uintl count = 0;

    //Count number of cubes left active
    for (uint iz = 0; iz < nz; iz++) {
        for (uint iy = 0; iy < ny; iy++) {
            for (uint ix = 0; ix < nx; ix++) {
                if (my3DArray[iz][iy][ix]) {
                    count++;
                }
            }
        }
    }
    cout << "count: " << count << endl;

}

template <typename T>
T**** create_4D_array( uint nz, uint ny, uint nx, uint nw, T fill) {

    T**** my4DArray = new T***[nz];
    for (uint iz = 0; iz < nz; iz++) {
        my4DArray[iz] = new T**[ny];
        for (uint iy = 0; iy < ny; iy++) {
            my4DArray[iz][iy] = new T*[nx];
            for (uint ix = 0; ix < nx; ix++) {
                my4DArray[iz][iy][ix] = new T[nw];
                for (uint iw = 0; iw < nw; iw++) {
                    my4DArray[iz][iy][ix][iw] = fill;
                }
            }
        }
    }

    return my4DArray;
};


void print_bool_4D_array(bool**** barr, uint nz, uint ny, uint nx, uint nw) {
    if (DEBUG) {
        for (uint iz = 0; iz < nz; iz++) {
            for (uint iw = 0; iw < nw; iw++) {
                cout << "iz : " << iz << ", iw: " << iw << endl;

                for (uint iy = 0; iy < ny; iy++) {
                    for (uint ix = 0; ix < nx; ix++) {
                        if (barr[iz][iy][ix][iw]) {
                            cout << "#";
                        }
                        else {
                            cout << ".";
                        }
                    }
                    cout << endl;
                }
            }
        }
    }
}

void part2(vector<string> slines) {
    //Same as part 1 but in 4dim
    
    //Get initial layout of grid as array of bools

    uint i_nx = slines[0].size();
    uint i_ny = slines.size();

    //Create an array that is larger by 6 on each direction
    const uint NCYCLES = 6;

    uint nw = 1 + 2 * NCYCLES;
    uint nz = 1 + 2 * NCYCLES;
    uint ny = i_ny + 2 * NCYCLES;
    uint nx = i_nx + 2 * NCYCLES;

    auto my4DArray = create_4D_array<bool>(nz, ny, nx, nw, false);

    // Initialize the array with init values
    uint iz = 0;
    uint iw = 0;

    for (uint iy = 0; iy < i_ny; iy++) {
        auto s0 = slines[iy];
        for (uint ix = 0; ix < i_nx; ix++) {
            auto c = s0[ix];
            if (c == '#') {
                my4DArray[iz + NCYCLES][iy + NCYCLES][ix + NCYCLES][iw + NCYCLES]= true;
            }
        }
    }

    cout << "Initialization complete" << endl;

    //Counting neighbors

    auto buf_arr = create_4D_array<bool>(nz, ny, nx,nw, false);

    auto count_neighb = create_4D_array<uint>(nz, ny, nx,nw, 0);

    for (uint icycle = 0; icycle < NCYCLES; icycle++) {

        cout << "cycle : " << icycle << endl;
        print_bool_4D_array(my4DArray, nz, ny, nx,nw);

        for (uint iz = 0; iz < nz; iz++) {

            uint iz0 = 0; if (iz > 0) iz0 = iz - 1;
            uint iz1 = nz - 1; if (iz < nz - 1) iz1 = iz + 1;

            for (uint iy = 0; iy < ny; iy++) {
                uint iy0 = 0; if (iy > 0) iy0 = iy - 1;
                uint iy1 = ny - 1; if (iy < ny - 1) iy1 = iy + 1;

                for (uint ix = 0; ix < nx; ix++) {
                    uint ix0 = 0; if (ix > 0) ix0 = ix - 1;
                    uint ix1 = nx - 1; if (ix < nx - 1) ix1 = ix + 1;

                    for (uint iw = 0; iw < nw; iw++) {
                        uint iw0 = 0; if (iw > 0) iw0 = iw - 1;
                        uint iw1 = nw - 1; if (iw < nw - 1) iw1 = iw + 1;

                        uint c = 0;
                        //Check all directions

                        for (auto iz2 = iz0; iz2 <= iz1; iz2++) {
                            for (auto iy2 = iy0; iy2 <= iy1; iy2++) {
                                for (auto ix2 = ix0; ix2 <= ix1; ix2++) {
                                    for (auto iw2 = iw0; iw2 <= iw1; iw2++) {
                                        if ((iz2 != iz || iy2 != iy || ix2 != ix || iw2 != iw) &&
                                            my4DArray[iz2][iy2][ix2][iw2]) {
                                            c++;
                                        }
                                    }
                                }
                            }
                        }

                        count_neighb[iz][iy][ix][iw] = c;
                    }
                }
            }
        }
        for (uint iz = 0; iz < nz; iz++) {
            for (uint iy = 0; iy < ny; iy++) {
                for (uint ix = 0; ix < nx; ix++) {
                    for (uint iw = 0; iw < nw; iw++) {
                        //Adjust based on value
                        auto c0 = count_neighb[iz][iy][ix][iw];
                        if (my4DArray[iz][iy][ix][iw]) {
                            if (c0 == 2 || c0 == 3) {
                                buf_arr[iz][iy][ix][iw] = true;
                            }
                            else {
                                buf_arr[iz][iy][ix][iw] = false;
                            }
                        }
                        else {
                            if (c0 == 3) {
                                buf_arr[iz][iy][ix][iw] = true;
                            }
                            else {
                                buf_arr[iz][iy][ix][iw] = false;
                            }
                        }
                    }
                }
            }
        }

        //new cycle
        swap(my4DArray, buf_arr);
    }

    cout << "final cycle : " << endl;
    print_bool_4D_array(my4DArray, nz, ny, nx,nw);

    uintl count = 0;

    //Count number of cubes left active
    for (uint iz = 0; iz < nz; iz++) {
        for (uint iy = 0; iy < ny; iy++) {
            for (uint ix = 0; ix < nx; ix++) {
                for (uint iw = 0; iw < nw; iw++) {
                    if (my4DArray[iz][iy][ix][iw]) {
                        count++;
                    }
                }
            }
        }
    }
    cout << "Part2 count: " << count << endl;

}

int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test.txt");
    //auto slines = get_lines_from_file("input.txt"); DEBUG = false;
    //part1(slines);

    //auto slines = get_lines_from_file("input_test.txt");
    auto slines = get_lines_from_file("input.txt"); DEBUG = false;
    part2(slines);

    return 0;
}