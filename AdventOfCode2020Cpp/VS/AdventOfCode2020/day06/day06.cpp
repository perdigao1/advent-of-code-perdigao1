// https://adventofcode.com/2020/day/5

#include <iostream> //cout and cin
#include <fstream> //read write files
#include <string>
#include <vector>
#include <map>
#include <list>
#include <tuple>
#include <sstream>
#include <regex>
#include <set>
#define uint unsigned int

using namespace std;

void part1(const vector<string>& stokens) {
    //For each token, get letters and put it in a set
    //Doesn't have to be ordered but for simplicity will use the ordered <set>

    int total_count = 0; // initialises counter

    for (auto s0 : stokens) {
        set<char> chars;

        for (auto c : s0) { //chars, note that can include line feed
            if ((int)c >= (int)'a' and (int)c <= (int)'z') {
                cout << c;
                chars.insert(c);
            }
        }
        cout << endl;

        size_t count_chars = chars.size();
        cout << "group count: " << count_chars <<endl;

        total_count += count_chars;
    }

    cout << "total_count : " << total_count;
}


void part2(const vector<string>& stokens) {
    //Need to split the lines in each group
    int total_count = 0; // initialises counter

    for (auto s0 : stokens) {

        map<char, int> char_count; //empty
        stringstream s1(s0);
        string sline;

        int nlines = 0;
        while (getline(s1, sline)) {
            for (auto c : sline) {
                if ((int)c >= (int)'a' and (int)c <= (int)'z') {
                    cout << c;
                    char_count[c]++; //will it work?
                }
            }
            nlines += 1;
            cout << endl;
        }
        cout << "nlines :" << nlines << endl;

        int n_all_yes = 0;
        for (auto c_count : char_count) {
            if (c_count.second == nlines) {
                n_all_yes += 1;
            }
        }
        cout << "n_all_yes :" << n_all_yes << endl;

        total_count += n_all_yes;
    }
    cout << "total_count: " << total_count << endl;

}


int main() {
    //Read data from file to list of strings

    //ifstream my_file("input_test.txt"); // Open the file
    ifstream my_file("input.txt"); // Open the file

    //Reads the whole file and tokenizes by double line feed

    vector<std::string> stokens;
    string sfile;

    if (my_file.is_open()) {

        stringstream ssbuffer;
        ssbuffer << my_file.rdbuf();
        string content = ssbuffer.str();

        sregex_token_iterator reg_end; //default token iterator initialises as end
        regex re("\\n\\n"); //delimiter
        
        for (sregex_token_iterator it(content.begin(), content.end(), re, -1); //iterator, -1 is to return parts that are not matched (in between matches)
            it != reg_end;
            ++it) {
            auto tok0 = (*it).str();
            stokens.push_back(tok0);
        }

        cout << "Whole file read and seperated in tokens." << endl;
    }
    else {
        cerr << "Unable to open file" << endl;
        return 1;
    }

    // Print the file
    cout << "*** file all lines ***" << endl;
    for (auto s : stokens) {
        cout << s << endl<<endl;
    }
    cout << "**********************" << endl << endl;

    //part1(stokens);
    part2(stokens);

    return 0;
}