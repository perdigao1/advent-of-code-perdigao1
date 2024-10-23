#pragma once
// shared code between days

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
#include <unordered_map>
#include <deque> //like vector+queue but items can be inserted/removed from start 
#include <algorithm> //max, min
#include <ranges>
#include <string_view>

//#define strings_t vector<string>
//#define uint unsigned int
//#define uintl unsigned long
//#define uintll unsigned long long
// 

using namespace std;

using strings_t = vector<string>;
using uint = unsigned int;
using uintl = unsigned long;
using uintll = unsigned long long;


//vector<string> get_lines_from_file(const char* filename) {
strings_t get_lines_from_file(const char* filename) {
    // Read data from file
    // returns a list of strings

    ifstream my_file(filename); // Open the file

    strings_t all_lines;
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
        exit(1);
    }
    //Remove last line if it is empty
    
    if (all_lines.size()>0){
        if (all_lines[all_lines.size() - 1].size() == 0) {
            all_lines.pop_back();
        }
        else {
            cout << "File has no lines" << endl;
        }
        // Print the file
        cout << "*** file all lines ***" << endl;
        for (auto s : all_lines) {
            cout << s << endl;
        }
        cout << "**********************" << endl << endl;
    }


    return all_lines;
}

struct match_group_pair {
    string match;
    strings_t groups;
};

vector< match_group_pair > regex_get_matches_and_their_groups(const string& text, const string& pattern) {
    regex regex_pattern(pattern);
    auto words_begin = sregex_iterator(text.begin(), text.end(), regex_pattern);
    auto words_end = sregex_iterator();

    vector< match_group_pair > matches;

    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch smatchg = *i;
        string smatch_string = smatchg[0].str();
        cout << "regex match: " << smatch_string << std::endl;
        //cout << "regex match[0]: " << smatchg[0].str() << std::endl;

        vector<string> groups;

        for (int j = 1; j < smatchg.size(); j++) {
            groups.push_back(smatchg[j].str());
        }

        matches.push_back({ smatchg.str(), groups });

    }
    return matches;
}

template <typename T> T** create2DArray(int rows, int cols) {
    T** array = new T * [rows];
    for (int i = 0; i < rows; ++i) {
        array[i] = new T[cols];
    }
    return array;
}

template <typename T> void delete2DArray(T** array, int rows) {
    for (int i = 0; i < rows; ++i) {
        delete[] array[i];
    }
    delete[] array;
}


//Split string by delimiter
vector<string> split(string str, char delim) {
    // Declare a vector to store the substrings
    vector<string> result;
    // Declare a string to store the current substring
    string current;
    // Loop through each character in the string
    for (char c : str) {
        // If the character is equal to the delimiter, then add the current substring to the result vector and clear it
        if (c == delim) {
            result.push_back(current);
            current.clear();
        }
        // Otherwise, append the character to the current substring
        else {
            current += c;
        }
    }
    // Add the last substring to the result vector
    result.push_back(current);
    // Return the result vector
    return result;
}



// Function to split a string by a delimiter
vector<string> split2(const string& str, char delimiter) {
    vector<string> result;

    // Use ranges to split the string
    auto splitView = string_view(str) | ranges::views::split(delimiter);

    for (const auto& subrange : splitView) {
        result.emplace_back(subrange.begin(), subrange.end());
    }

    return result;
}

