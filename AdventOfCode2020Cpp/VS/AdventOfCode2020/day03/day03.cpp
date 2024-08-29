// https://adventofcode.com/2020/day/3

#include <iostream> //cout and cin
#include <fstream> //read write files
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <regex>

using namespace std;


void part1(const vector<string>& all_lines) {
    // get number of cols and rows
    size_t nrows = all_lines.size();
    size_t ncols = all_lines[0].size();

    cout << "nrows:" << nrows << " , ncols:" << ncols << endl;

    // create bool array with trees marked as true
    bool** map = new bool*[nrows];
    // initialise with false
    for (int i = 0; i < nrows; i++) {
        map[i] = new bool[ncols];
        for (int j = 0; j < ncols; j++) {
            map[i][j] = false;
            if (all_lines[i][j] == '#') {
                //tree
                map[i][j] = true;
            }
        }
    }
    cout << "bool map created" << endl;

    // simulate
    cout << "simulate" << endl;
    int ic = 0; // start col

    int tree_hit_count = 0;

    for (int ir = 0; ir < nrows; ir++) {
        if (map[ir][ic] == true) tree_hit_count++; //hit tree
        ic += 3;
        if (ic >= ncols) ic = ic % ncols; //map repetition
    }
    cout << "tree_hit_count:" << tree_hit_count << endl;
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

    part1(all_lines);

	return 0;
}