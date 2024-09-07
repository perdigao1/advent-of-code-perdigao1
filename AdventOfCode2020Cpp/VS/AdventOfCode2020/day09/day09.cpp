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
#include <unordered_map>
#include <deque>
#include <algorithm>

#define uint unsigned int

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

vector< pair<string, vector<string>> > regex_get_matches_and_their_groups(const string& text, const string& pattern) {
    regex regex_pattern(pattern);
    auto words_begin = sregex_iterator(text.begin(), text.end(), regex_pattern);
    auto words_end = sregex_iterator();

    vector< pair<string, vector<string>> > matches;

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

void part1(const vector<uint>& number_list, uint i_prev_numbers) {
    cout << "part1()" << endl;

    //Find the first number in the list that does not follow rule
    //Deque is must start with 25 elements

    deque<uint> prev_numbers;

    //best strategy is probably to iterate through elements

    for (const auto& n0 : number_list) {
        if (prev_numbers.size() < i_prev_numbers) {
            //simply add element to end of deque list
            prev_numbers.push_back(n0);
        }
        else {
            bool is_sum = false;
            // Check element is sum of pairs from others
            for (uint i = 0; i < prev_numbers.size() - 1; i++) {
                uint ni = prev_numbers[i];
                for (uint j = i + 1; j < prev_numbers.size(); j++) {
                    uint nj = prev_numbers[j];

                    if (ni + nj == n0) {
                        is_sum = true;
                        cout << n0 << " is sum of two previous numbers " << ni << " and " << nj << endl;
                        break;
                    }
                }
                if (is_sum) break;
            }
            if (is_sum) {
                //value is ok, add to list for next iteration, and remove the first element
                prev_numbers.push_back(n0);
                prev_numbers.pop_front();
            }
            else {
                cout << "Value " << n0 << " is not a pair sum of previous" << endl;

                //stop here as part1 requirement
                exit(0);
            }

        }
    }
}


void part2(const vector<uint>& number_list, uint i_prev_numbers) {
    // Do part 1 to find the index of the wrong number
    cout << "part2()" << endl;

    //Find the first number in the list that does not follow rule
    //Deque is must start with 25 elements

    deque<uint> prev_numbers;

    int wrong_number_index = -1;
    uint wrong_number = 0;

    //best strategy is probably to iterate through elements

    //for (const auto& n0 : number_list) {
    for (uint i_nl=0; i_nl<number_list.size(); i_nl++){
        uint n0 = number_list[i_nl];

        if (prev_numbers.size() < i_prev_numbers) {
            //simply add element to end of deque list
            prev_numbers.push_back(n0);
        }
        else {
            bool is_sum = false;
            // Check element is sum of pairs from others
            for (uint i = 0; i < prev_numbers.size() - 1; i++) {
                uint ni = prev_numbers[i];
                for (uint j = i + 1; j < prev_numbers.size(); j++) {
                    uint nj = prev_numbers[j];

                    if (ni + nj == n0) {
                        is_sum = true;
                        cout << n0 << " is sum of two previous numbers " << ni << " and " << nj << endl;
                        break;
                    }
                }
                if (is_sum) break;
            }
            if (is_sum) {
                //value is ok, add to list for next iteration, and remove the first element
                prev_numbers.push_back(n0);
                prev_numbers.pop_front();
            }
            else {
                cout << "Value " << n0 << " is not a pair sum of previous" << endl;

                //stop here as part1 requirement
                wrong_number_index = i_nl;
                wrong_number = n0;
                break;
            }

        }
    }

    if (wrong_number_index != -1) {
        cout << "wrong_number_index: " << wrong_number_index << " , wrong_number: "<< wrong_number << endl;

        // Check for contiguous numbers that added up give the number
        //start from the number index-1 and decrease index
        
        for (uint nj = wrong_number_index - 1; nj >= 1; nj--) { //upper limit
            uint upper_number = number_list[nj];

            //initialise
            uint max_value = upper_number;
            uint min_value = upper_number;
            uint sum = upper_number; 

            for (uint ni = nj - 1; ni >= 0; ni--) {
                uint bottom_value = number_list[ni];
                sum += bottom_value;
                max_value = max(max_value, bottom_value);
                min_value = min(min_value, bottom_value);

                //check if sum corresponds to number
                if (sum == wrong_number) {
                    cout << "Found contigous numbers that give the correct sum.  min,max " << min_value << " , " << max_value << endl;
                    cout << "Their sum is " << min_value+max_value;
                    exit(0);
                }
                else if (sum>wrong_number){
                    break;
                }
            }
        }

    }
}

int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test.txt");
    //uint preamb = 5;

    auto slines = get_lines_from_file("input.txt");
    uint preamb = 25;
    

    vector<uint> number_list;
    for (auto s : slines) {
        
        try {
            auto i = stoi(s);
            number_list.push_back(i);
        }
        catch(exception e){
            //do nothing
        }
    }
    //part1(number_list, preamb);
    part2(number_list, preamb);

    return 0;
}