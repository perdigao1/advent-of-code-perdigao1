// https://adventofcode.com/2020/day/10

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

#define uint unsigned int
//#define uint unsigned long
//#define uint unsigned long

using namespace std;

vector<string> get_lines_from_file(const char* filename) {
    // Read data from file
    // returns a list of strings

    ifstream my_file(filename); // Open the file

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
        exit(1);
    }

    // Print the file
    cout << "*** file all lines ***" << endl;
    for (auto s : all_lines) {
        cout << s << endl;
    }
    cout << "**********************" << endl << endl;

    return all_lines;
}

struct match_group_pair {
    string match;
    vector<string> groups;
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

void part1(set<uint> number_set) {
    
    map<uint, uint> diffs_count;

    auto it0 = number_set.begin();
    auto n0 = *(it0);
    diffs_count[n0]++; //first jolt is a jolt difference

    for (auto it1 = next(it0) ; it1 != number_set.end(); ++it1) {
        auto n1 = *(it1);
        cout << "n0, n1 : " << n0 << ","<< n1 << endl;

        auto diff = n1 - n0;
        diffs_count[diff]++;

        n0 = n1; //prepare for next iteration
    }
    //Last diff
    diffs_count[3]++;
    
    cout << "diffs_count" << endl;
    for (auto d : diffs_count) {
        cout << d.first << "," << d.second << endl;
    }

    cout << "solution j1*j3 : " << diffs_count[1] * diffs_count[3];
}

unsigned long long dfs_count(const vector<uint>& number_vector, uint iloc, uint depth, map<uint, unsigned long long>& count_mem) {
    //cout << string(depth*2, ' ') << "dfs() iloc:" << iloc << " , depth:" << depth << endl;
    //Get options from this location to traverse
    if (iloc == number_vector.size() - 1) {
        return 1; //last jolt
    }

    unsigned long long ncount = 0;
    if (count_mem.find(iloc) == count_mem.end()) {
        auto v0 = number_vector[iloc];
        

        for (uint i = iloc + 1; i < number_vector.size(); i++) {
            auto diff = number_vector[i] - v0;

            if (diff > 3) {
                break; //stop trying other jolts
            }
            else {
                ncount += dfs_count(number_vector, i, depth + 1, count_mem);
            }
        }
        count_mem[iloc] = ncount; //memorize this count
    }
    else {
        ncount = count_mem[iloc];
    }
    return ncount;
}

void part2(set<uint> number_set) {
    //number of distinct ways?

    // Add first jolt
    number_set.insert(0);

    //Convert to vector so that can be tranversed with index

    vector<uint> number_vector(number_set.begin(), number_set.end());
    //can do this!! Uses a range constructor

    map <uint, unsigned long long> count_mem;

    unsigned long long ncount = dfs_count(number_vector, 0, 0, count_mem); //start from first jolt

    cout << "ncount : " << ncount;
}

int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test0.txt"); //1:7, 3:5 , 8
    //auto slines = get_lines_from_file("input_test1.txt"); // 1:22, 3:10 , 19208
    auto slines = get_lines_from_file("input.txt");

    //Convert all numbers to (ordered) set of ints
    set<uint> number_set; //by default values will be stored in ascending order
    for (auto s : slines) {
        try {
            auto i = stoi(s);
            number_set.insert(i);
        }
        catch (exception e) {
            //do nothing
        }
    }
    //part1(number_set);
    part2(number_set);

    return 0;
}