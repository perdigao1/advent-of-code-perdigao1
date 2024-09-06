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


void part1(vector<string> slines) {
    cout << "part1()" << endl;

    vector<bool> instr_has_run(slines.size(), false);
    list<uint> ip_log;

    bool has_detected_double_run = false;

    uint ip = 0; //instruction pointer
    int acc = 0;

    //Start at first instruction ip=0 and go on
    bool has_rerun_instr_detected = false;
    
    do {
        if (instr_has_run[ip] == true) {
            has_rerun_instr_detected = true; //leave loop
            cout << "ip: " << ip << " had run previously" << endl;
            
        }
        else {
            instr_has_run[ip] = true;
            ip_log.push_back(ip);

            string instr = slines[ip];
            cout << "ip: " << ip << " , instr: " << instr << " , acc: " << acc << endl;

            auto split_res = regex_get_matches_and_their_groups(instr, "(\\w+) ([+-]\\d+)");

            // check is ok
            if (split_res.size() == 1) {
                string command = split_res[0].second[0];
                int value = stoi(split_res[0].second[1]);

                //Check command
                if (command == "acc") {
                    //add to acc and jumps line
                    acc += value;
                    ip++;

                }
                else if (command == "nop") {
                    ip++;
                }
                else if (command == "jmp") {
                    ip += value;
                }
                else {
                    cerr << "Not a valid instruction" << endl;
                }
            }
        }
    } while (!has_rerun_instr_detected);
    
    cout << "acc: " << acc;

}


void part2(vector<string> slines) {
    uint n_instructions = slines.size();

    cout << "part2()" << endl;

    uint instr_to_change = 0;

    do {
        auto ch_line = slines[instr_to_change];

        cout << "instr_to_change: " << instr_to_change << " , sline: " << ch_line << endl;

        bool do_run = false;
        string old_instr = ch_line;

        if (ch_line.find("nop") != ch_line.npos) {
            string new_instr = ch_line.replace(0, 3, "jmp");
            slines[instr_to_change] = new_instr;
            do_run = true;
            cout << "Replaced instruction nop to jmp. Now trying run" << endl;
        }
        else if (ch_line.find("jmp") != ch_line.npos) {
            string new_instr = ch_line.replace(0, 3, "nop");
            slines[instr_to_change] = new_instr;
            do_run = true;
            cout << "Replaced instruction jmp to jmp. Now trying run" << endl;
        }

        if (do_run) {
            cout << "replaced instruction. now trying run" << endl;

            vector<bool> instr_has_run(slines.size(), false);
            //list<uint> ip_log;

            uint ip = 0; //instruction pointer
            int acc = 0;

            //Start at first instruction ip=0 and go on
            bool has_rerun_instr_detected = false;

            do {
                if (instr_has_run[ip] == true) {
                    has_rerun_instr_detected = true; //leave loop
                    cout << "ip: " << ip << " had run previously" << endl;

                }
                else {
                    instr_has_run[ip] = true;
                    //ip_log.push_back(ip);

                    string instr = slines[ip];
                    cout << "ip: " << ip << " , instr: " << instr << " , acc: " << acc << endl;

                    auto split_res = regex_get_matches_and_their_groups(instr, "(\\w+) ([+-]\\d+)");

                    // check is ok
                    if (split_res.size() == 1) {
                        string command = split_res[0].second[0];
                        int value = stoi(split_res[0].second[1]);

                        //Check command
                        if (command == "acc") {
                            //add to acc and jumps line
                            acc += value;
                            ip++;

                        }
                        else if (command == "nop") {
                            ip++;
                        }
                        else if (command == "jmp") {
                            ip += value;
                        }
                        else {
                            cerr << "Not a valid instruction" << endl;
                        }
                    }
                }
            } while (!has_rerun_instr_detected && ip != n_instructions);

            if (ip == n_instructions) {
                cout << "Program terminated normally. instr_to_change: " << instr_to_change << endl;
                cout << "acc: " << acc << endl;
                break;
            }
            else if (has_rerun_instr_detected) {
                cout << "Program repeating instructions" << endl;
            }

            //restore instruction
            slines[instr_to_change] = old_instr;
        }
        instr_to_change++;

    } while (instr_to_change < n_instructions);




}

int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test.txt");
    auto slines = get_lines_from_file("input.txt");

    //part1(slines); //ok
    part2(slines); //ok

    return 0;
}