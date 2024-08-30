// https://adventofcode.com/2020/day/3

#include <iostream> //cout and cin
#include <fstream> //read write files
#include <string>
#include <vector>
#include <map>
#include <list>
#include <tuple>
#include <sstream>
#include <regex>

using namespace std;


void part1(const vector<string>& all_lines) {
    cout << "part1" << endl;
    // get number of cols and rows
    size_t nrows = all_lines.size();
    size_t ncols = all_lines[0].size();

    cout << "nrows:" << nrows << " , ncols:" << ncols << endl;

    // create bool array with trees marked as true
    bool** tree_map = new bool*[nrows];
    // initialise with false
    for (int i = 0; i < nrows; i++) {
        tree_map[i] = new bool[ncols];
        for (int j = 0; j < ncols; j++) {
            tree_map[i][j] = false;
            if (all_lines[i][j] == '#') {
                //tree
                tree_map[i][j] = true;
            }
        }
    }
    cout << "bool map created" << endl;

    // simulate
    cout << "simulate" << endl;
    int ic = 0; // start col

    int tree_hit_count = 0;

    for (int ir = 0; ir < nrows; ir++) {
        if (tree_map[ir][ic] == true) tree_hit_count++; //hit tree
        ic += 3;
        if (ic >= ncols) ic = ic % ncols; //map repetition
    }
    cout << "tree_hit_count:" << tree_hit_count << endl;
}

void part2(const vector<string>& all_lines) {

    cout << "part2" << endl;
    // get number of cols and rows
    size_t nrows = all_lines.size();
    size_t ncols = all_lines[0].size();

    cout << "nrows:" << nrows << " , ncols:" << ncols << endl;

    // create bool array with trees marked as true
    bool** tree_map = new bool* [nrows];
    // initialise with false
    for (int i = 0; i < nrows; i++) {
        tree_map[i] = new bool[ncols];
        for (int j = 0; j < ncols; j++) {
            tree_map[i][j] = false;
            if (all_lines[i][j] == '#') {
                //tree
                tree_map[i][j] = true;
            }
        }
    }
    cout << "bool map created" << endl;

    // simulate
    cout << "simulate" << endl;

    // https://adventofcode.com/2020/day/3#part2
    //int slopes[5][2] = { {1,1},{3,1},{5,1},{7,1},{1,2} };
    list< tuple<int,int> > slopes = { {1,1},{3,1},{5,1},{7,1},{1,2} };

    long tree_hit_multiplier = 1; //initialise

    for (const auto& tp : slopes) {
        int step_right = get<0>(tp);
        int step_down = get<1>(tp);

        cout << "step_right:" << step_right << " , step_down:" << step_down << endl;
        
        int ic = 0; // start col
        int tree_hit_count = 0;

        for (int ir = 0; ir < nrows; ir+= step_down) {
            if (tree_map[ir][ic] == true) tree_hit_count++; //hit tree
            ic += step_right;
            if (ic >= ncols) ic = ic % ncols; //map repetition
        }
        cout << "tree_hit_count:" << tree_hit_count << endl;

        tree_hit_multiplier *= tree_hit_count;
        cout << "tree_hit_multiplier:" << tree_hit_multiplier << endl;

    }
}

int main() {
    //Read data from file to list of strings

    //ifstream my_file("input_test.txt"); // Open the file
    ifstream my_file("input.txt"); // Open the file

    vector<string> all_lines;
    string sline;

    if (my_file.is_open()) {
        //Read all lines and stores in all_lines vector
        while (getline(my_file, sline)) {
            all_lines.push_back(sline);
        }
        my_file.close();
        cout << "Whole file read." << endl;

    }
    else {
        cerr << "Unable to open file" << endl;
        return 1;
    }

    // Print the file
    cout << "*** file all lines ***" << endl;
    for (auto s : all_lines) {
        cout << s << endl;
    }
    cout << "**********************" << endl;

    //part1(all_lines);
    part2(all_lines);

	return 0;
}