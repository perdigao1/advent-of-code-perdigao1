// https://adventofcode.com/2020/day/4

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
    // Get passports informations
    //Seperate fields, probably using regex is best

    vector< map<string, string> > passports;

    regex regex_pattern("([a-z]+):([#a-z0-9]+)");
    //smatch regex_match; //to collect regex match results

    map<string, string> *passport_p = new map<string,string>;
        
    for (const auto& sline : all_lines) {
        cout << "Line: " << sline << endl;
        if (sline.size() == 0) {
            cout << "Line is empty" << endl;
            if ((*passport_p).size() > 0) {
                passports.push_back(*passport_p);
                cout << "End of passport info, added to list." << endl;

                //prepare a new one
                passport_p = new map<string, string>;
            }else {
                cout << "No passport info" << endl;
            }
        }
        else {

            /*
            if (regex_search(sline, regex_match, regex_pattern)) {
                if (regex_match.size() != 0) {
                    // get matches and add to the passport map
                    (*passport_p)[regex_match[1].str()] = regex_match[2].str();
                }else {
                    cerr << "No matches found and not newline" << endl;
                }
            }*/
            auto words_begin = sregex_iterator(sline.begin(), sline.end(), regex_pattern);
            auto words_end = sregex_iterator();

            //iterates thorough each match and inner elements
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                smatch match = *i;
                cout << "Match: " << match.str() << endl;
                // gets elements
                cout << "--- " << match[1].str() << " " << match[2].str() << endl;
                (*passport_p)[match[1].str()] = match[2].str();
            }
        }
    }
    cout << "All lines parsed" << endl;

    if ((*passport_p).size() > 0) {
        cout << "Last passport, add to list" << endl <<endl;
        passports.push_back(*passport_p);
    }

    // print passports
    // Iterate through the list of maps and print the contents
    int count_valid = 0;
    cout << "Print passports" << endl;
    for (const auto& map : passports) {
        int n_fields = 0;
        int has_cid = false;

        for (const auto& pair : map) {
            std::cout << pair.first << ": " << pair.second << endl;

            auto k = pair.first;
            if (k == "byr") {
                n_fields++;
            }
            else if (k == "iyr") {
                n_fields++;
            }
            else if (k == "eyr") {
                n_fields++;
            }
            else if (k == "hgt") {
                n_fields++;
            }
            else if (k == "hcl") {
                n_fields++;
            }
            else if (k == "ecl") {
                n_fields++;
            }
            else if (k == "pid") {
                n_fields++;
            }
            else if (k == "cid") {
                n_fields++;
                has_cid = true;
            }
        }

        cout << "n_fields: " << n_fields << endl;

        if (n_fields == 8) {
            cout << "Passport valid because has all 8 fields" << endl;
            count_valid++;
        }
        else if (!has_cid && n_fields > 6) {
            cout << "Passport has missing fields, one of them being cid and maybe one more. Passport is valid" << endl;
            cout << endl;
            count_valid++;
        }
        else {
            cout << "Passport invalid" << endl;
        }
        
        std::cout << endl;
    }

    // Process passports to get answers


    cout << "Count valid passports:" << count_valid << endl;

}

int get_4digit_year(const string& s) {
    if (s.size() != 4) {
        return -1;
    }
    regex regex_pattern("[0-9]{4}$");
    if (regex_search(s, regex_pattern)) {
        return stoi(s);
    }
    return -1;
}
void part2(const vector<string>& all_lines) {
    // Get passports informations
    //Seperate fields, probably using regex is best

    vector< map<string, string> > passports;

    regex regex_pattern("([a-z]+):([#a-z0-9]+)");
    //smatch regex_match; //to collect regex match results

    map<string, string>* passport_p = new map<string, string>;

    for (const auto& sline : all_lines) {
        //cout << "Line: " << sline << endl;
        if (sline.size() == 0) {
            //cout << "Line is empty" << endl;
            if ((*passport_p).size() > 0) {
                passports.push_back(*passport_p);
                //cout << "End of passport info, added to list." << endl;

                //prepare a new one
                passport_p = new map<string, string>;
            }
            else {
                //cout << "No passport info" << endl;
            }
        }
        else {

            /*
            if (regex_search(sline, regex_match, regex_pattern)) {
                if (regex_match.size() != 0) {
                    // get matches and add to the passport map
                    (*passport_p)[regex_match[1].str()] = regex_match[2].str();
                }else {
                    cerr << "No matches found and not newline" << endl;
                }
            }*/
            auto words_begin = sregex_iterator(sline.begin(), sline.end(), regex_pattern);
            auto words_end = sregex_iterator();

            //iterates thorough each match and inner elements
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                smatch match = *i;
                //cout << "Match: " << match.str() << endl;
                // gets elements
                //cout << "--- " << match[1].str() << " " << match[2].str() << endl;
                (*passport_p)[match[1].str()] = match[2].str();
            }
        }
    }
    cout << "All lines parsed" << endl;

    if ((*passport_p).size() > 0) {
        cout << "Last passport, add to list" << endl << endl;
        passports.push_back(*passport_p);
    }

    // print passports
    // Iterate through the list of maps and print the contents
    int count_valid = 0;
    cout << endl;
    cout << "Print passports" << endl;
    list<int> valid_passports;

    //for (const auto& key_values : passports) {
    for (int ipassp=0; ipassp < passports.size(); ipassp++){
        auto key_values = passports[ipassp];
        int n_fields = 0; //excluding cid
        //int has_cid = false;

        bool is_valid = true; //defaults to being a valid passport
        cout << "ipassp:" << ipassp << endl;

        for (const auto& pair : key_values) {
             cout << pair.first << ": " << pair.second << endl;

            auto k = pair.first;
            auto s2 = pair.second;
            if (k == "byr") {
                n_fields++;
                int yr = get_4digit_year(s2);
                if (yr<0) {
                    is_valid = false;
                    cout << "Bad byr" << endl;
                }
                else {
                    if (yr < 1920 || yr>2002) {
                        is_valid = false;
                        cout << "Bad byr:"<< yr << " not within [1920,2002]" << endl;
                    }
                }
            }
            else if (k == "iyr") {
                n_fields++;
                int yr = get_4digit_year(s2);
                if (yr < 0) {
                    is_valid = false;
                    cout << "Bad iyr" << endl;
                }
                else {
                    if (yr < 2010 || yr>2020) {
                        is_valid = false;
                        cout << "Bad iyr:" << yr << " not within [2010,2020]" << endl;
                    }
                }
            }
            else if (k == "eyr") {
                n_fields++;
                int yr = get_4digit_year(s2);
                if (yr < 0) {
                    is_valid = false;
                    cout << "Bad eyr" << endl;
                }
                else {
                    if (yr < 2020 || yr>2030) {
                        is_valid = false;
                        cout << "Bad eyr:" << yr << " not within [2020,2030]" << endl;
                    }
                }
            }
            else if (k == "hgt") {
                n_fields++;
                //height in cm or in
                //cm
                size_t s_cm = s2.find("cm");
                if (s_cm != string::npos) {
                    //crop
                    int i = stoi(s2.substr(0,s_cm));
                    if (i<150 || i>193) {
                        is_valid = false;
                        cout << "Bad hgt in cm:" << i << " not within [150,193]" << endl;
                    }
                }
                else {
                    size_t s_in = s2.find("in");
                    if (s_in != string::npos) {
                        //crop
                        int i = stoi(s2.substr(0, s_in));
                        if (i < 59 || i>76) {
                            is_valid = false;
                            cout << "Bad hgt in in:" << i << " not within [59,76]" << endl;
                        }
                    }
                    else {
                        is_valid = false;
                        cout << "Bad hgt. No cm or in." << endl;
                    }

                }
            }
            else if (k == "hcl") {
                n_fields++;
                //Use regex to check for matching pattern
                regex regex_hcl_pattern("#[0-9a-f]{6}$");

                if (!regex_search(s2, regex_hcl_pattern)) {
                    is_valid = false;
                    cout << "Bad hcl. not valid pattern" << endl;
                }
            }
            else if (k == "ecl") {
                n_fields++;
                //eye color must match one of amb blu brn gry grn hzl oth
                if (s2 != "amb" && s2 != "blu" && s2 != "brn" && s2 != "gry" &&
                    s2 != "grn" && s2 != "hzl" && s2 != "oth") {
                    is_valid = false;
                    cout << "Bad ecl. No match to possible values amb blu brn gry grn hzl oth" << endl;
                }
            }
            else if (k == "pid") {
                n_fields++;
                // 9 digit number
                regex regex_pid_pattern("^\s*[0-9]{9}$");
                if (!regex_search(s2, regex_pid_pattern)) {
                    is_valid = false;
                    cout << "Bad pid. No match to pattern of 9 digits" << endl;
                }
            }
            //else if (k == "cid") {
            //    n_fields++;
            //    has_cid = true;
            //}
        }
        
        cout << "n_fields: " << n_fields << endl;

        //if ((has_cid && n_fields<=7) || (!has_cid && n_fields<=6)){
        //    is_valid = false;
        //    cout << "Bad n_fields and cid." << endl;
        //}
        if (n_fields < 7) {
            is_valid = false;
            cout << "Bad n_fields < 7." << endl;
        }

        if (is_valid) {
            count_valid++;
            cout << "Passport " << ipassp << " is valid" << endl;
            valid_passports.push_back(ipassp);
        }
        else {
            cout << "Passport  " << ipassp << " is invalid" << endl;
        }
        std::cout << endl;
    }

    // Process passports to get answers

    cout << "Valid passports indices" << endl;
    for (const auto& item : valid_passports) {
        std::cout << item << ", ";
    }
    cout << endl;


    cout << "Count valid passports:" << count_valid << endl;

}


int main() {
    //Read data from file to list of strings

    //ifstream my_file("input_test.txt"); // Open the file
    ifstream my_file("input.txt"); // Open the file
    //ifstream my_file("test_invalid.txt"); // Open the file
    //ifstream my_file("test_valid.txt");

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