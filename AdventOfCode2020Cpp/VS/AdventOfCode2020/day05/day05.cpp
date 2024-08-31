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

void part1(const vector<string>& all_lines) {
    // string is binary of 7 characters F or B
    // followed by 3 charcters R or L
    
    int seatid_max=0;
    
    for (const auto& sline : all_lines) {
        cout << "Line: " << sline << endl;

        regex regex_pattern("^([FB]{7})([RL]{3})$");
        smatch regex_match_str; //to collect regex match results

        if (regex_match(sline, regex_match_str, regex_pattern)) {
            cout << "regex found matches. regex_match_str.size()=" << regex_match_str.size() << endl;

        }
        if (regex_match_str.size() == 3) {
            //cout << regex_match_str[1] << endl;
            //cout << regex_match_str[2] << endl;

            int rowid=0;
            //Convert letters to binary
            for (auto c0 : regex_match_str[1].str()) {
                cout << c0;
                if (c0 == 'B') {
                    rowid += 1;
                }
                rowid = rowid << 1;
            }
            rowid = rowid >> 1; //reverse extra (last) shift
            cout << endl;
            cout << "rowid: " << rowid << endl;

            int colid = 0;
            //Convert letters to binary
            for (auto c0 : regex_match_str[2].str()) {
                cout << c0;
                if (c0 == 'R') {
                    colid += 1;
                }
                colid = colid << 1;
            }
            colid = colid >> 1; //reverse extra (last) shift
            cout << endl;
            cout << "colid: " << colid << endl;

            int seatid = rowid *8 + colid;

            cout << "seatid: " << seatid << endl;

            seatid_max = max(seatid, seatid_max);
        }
        cout << endl;
    }
    cout << "seatid_max: " << seatid_max << endl;

}


void part2(const vector<string>& all_lines) {
    // string is binary of 7 characters F or B
    // followed by 3 charcters R or L

    set<uint> seatids;
    const uint first_seatid = 89;
    const uint last_seatid = 989;

    for (const auto& sline : all_lines) {
        cout << "Line: " << sline << endl;

        regex regex_pattern("^([FB]{7})([RL]{3})$");
        smatch regex_match_str; //to collect regex match results

        if (regex_match(sline, regex_match_str, regex_pattern)) {
            cout << "regex found matches. regex_match_str.size()=" << regex_match_str.size() << endl;

            if (regex_match_str.size() == 3) {
                //cout << regex_match_str[1] << endl;
                //cout << regex_match_str[2] << endl;

                uint rowid = 0;
                //Convert letters to binary
                for (auto c0 : regex_match_str[1].str()) {
                    //cout << c0;
                    if (c0 == 'B') {
                        rowid += 1;
                    }
                    rowid = rowid << 1;
                }
                rowid = rowid >> 1; //reverse extra (last) shift
                //cout << endl;
                //cout << "rowid: " << rowid << endl;

                uint colid = 0;
                //Convert letters to binary
                for (auto c0 : regex_match_str[2].str()) {
                    //cout << c0;
                    if (c0 == 'R') {
                        colid += 1;
                    }
                    colid = colid << 1;
                }
                colid = colid >> 1; //reverse extra (last) shift
                //cout << endl;
                //cout << "colid: " << colid << endl;

                uint seatid = rowid * 8 + colid;

                cout << "seatid: " << seatid << endl;
                
                // TODO: Register this seatid
                seatids.insert(seatid);

            }
            cout << endl;



        }
    }
    cout << "Print all seat ids" << endl;
    for (const auto& id0 : seatids) {
        cout << id0 << ", ";
    }
    cout << endl;

    //Check which value is missing
    uint missing_seatid = 0;
    for (uint i = first_seatid; i <= last_seatid; i++) {
        if (seatids.find(i) == seatids.end()) {
            missing_seatid = i;
            cout << "missing seat id " << missing_seatid << endl;
        }
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
    cout << "**********************" << endl << endl;

    //part1(all_lines);
    part2(all_lines);

    return 0;
}