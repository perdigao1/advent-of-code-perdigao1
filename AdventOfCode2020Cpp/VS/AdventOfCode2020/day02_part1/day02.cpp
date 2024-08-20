//#include <cstdio>
#include <iostream> //cout and cin
#include <fstream> //read write files
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <regex>

using namespace std;

void part1(const vector<string>& all_lines) {

    // Use regex to seperate components
    // with help of https://regex101.com/

    regex regex_pattern("([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)");
    smatch regex_match; //to collect regex match results

    int n_valid = 0;

    for (const auto& s : all_lines) {
        cout << "Line: " << s << endl;
        if (std::regex_search(s, regex_match, regex_pattern)) {
            if (regex_match.size() != 5) {
                cerr << "Error with string: " << s << " . Not enough groups. Only "
                    << regex_match.size() << endl;
            }
            else {
                // first digits
                int i0 = stod(regex_match[1].str());
                int i1 = stod(regex_match[2].str());
                char rep_char = regex_match[3].str().c_str()[0];
                string spass = regex_match[4].str();

                cout << "range " << i0 << " to " << i1 << " , " << " letter " << rep_char << endl;

                //Find if string contains the number of letters
                // Use std::count
                int n_occ = count(spass.begin(), spass.end(), rep_char);

                cout << "Character " << rep_char << " found " << n_occ << " times." << endl;

                if (n_occ >= i0 && n_occ <= i1) {
                    n_valid++;
                    cout << "Valid, n_valid is now " << n_valid << endl;
                }
                else {
                    cout << "Not valid" << endl;
                }


            };

        }
        else {
            cerr << "Error with string: " << s << " . No matches found." << endl;
        }
    }

    cout << "Result n_valid=" << n_valid << endl << endl;
}

void part2(const vector<string>& all_lines) {

    // Use regex to seperate components
    // with help of https://regex101.com/

    regex regex_pattern("([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)");
    smatch regex_match; //to collect regex match results

    int n_valid = 0;

    for (const auto& s : all_lines) {
        cout << "Line: " << s << endl;
        if (std::regex_search(s, regex_match, regex_pattern)) {
            if (regex_match.size() != 5) {
                cerr << "Error with string: " << s << " . Not enough groups. Only "
                    << regex_match.size() << endl;
            }
            else {
                // first digits
                int i0 = stod(regex_match[1].str());
                int i1 = stod(regex_match[2].str());
                char rep_char = regex_match[3].str().c_str()[0];
                string spass = regex_match[4].str();

                cout << "i0:" << i0 << " i1:" << i1 << " , " << " letter " << rep_char << endl;

                //Check if valid
                i0--;// indices in txt start at 1, convert to zero-start
                i1--;

                if (
                    (spass[i0] == rep_char && spass[i1]!= rep_char) ||
                    (spass[i0] != rep_char && spass[i1] == rep_char) ) {
                    n_valid++;
                    cout << "Line is valid. n_valid:" << n_valid << endl;
                }
                else {
                    cout << "Not valid" << endl;
                }
            }
        }
        else {
            cerr << "Error with string: " << s << " . No matches found." << endl;
        }
    }

    cout << "Result n_valid=" << n_valid << endl << endl;
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

