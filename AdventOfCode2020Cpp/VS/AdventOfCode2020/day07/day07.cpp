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

struct Bag_rule {
    uint i_primary_bag=0;
    list<pair<uint, uint>> contains; //qty , bag_index
};


void part1(const list<Bag_rule>& bag_rules, unordered_map<string, uint> bags_to_uint) {
    cout << "part1()" << endl;

    uint i_shinygold = bags_to_uint["shiny gold"];
    cout << "shiny gold index: " << i_shinygold << endl;

    // iteratively find the bags that have gold, mark them with boolean

    uint n_bags = bags_to_uint.size();

    vector<bool> has_gold(n_bags,false); //initialise with false

    has_gold[i_shinygold] = true;

    bool has_changed;

    uint iter = 0;

    do {
        has_changed = false;

        for (auto br : bag_rules) {
            // check if the contains has any of others that have shiny gold

            if (!has_gold[br.i_primary_bag]) {

                for (auto cont0 : br.contains) {
                    //first value is count
                    //second is the bag typ index
                    if (has_gold[cont0.second]) {
                        has_gold[br.i_primary_bag] = true;
                        has_changed = true;
                    }
                }
            }
        }
        iter++;
        cout << "iter: " << iter << " ,  has_changed: " << has_changed << endl;

    }while (has_changed);
    
    // count

    uint count_bags = 0;
    for (auto b : has_gold) {
        if (b) count_bags++;
    }
    count_bags--; //remove the shiny gold bag from count

    cout << endl <<"count_bags: " << count_bags <<endl<<endl;

}


uint get_number_of_bags_in(uint bag_idx, const list<Bag_rule>& bag_rules) {
    uint count_bags = 0;

    for (auto br : bag_rules) {
        if (br.i_primary_bag == bag_idx) {
            for (auto c0 : br.contains) {
                count_bags += c0.first *(1+ get_number_of_bags_in(c0.second, bag_rules));
            }
            break;
        }
    }
    cout << "bag " << bag_idx << " contains " << count_bags << endl;

    return count_bags;
}

void part2(const list<Bag_rule>& bag_rules, unordered_map<string, uint> bags_to_uint) {
    cout << "part1()" << endl;

    uint i_shinygold = bags_to_uint["shiny gold"];
    cout << "shiny gold index: " << i_shinygold << endl;

    //This is a recursion problem. Requires a seperate function to handle it
    //Start with the shiny gold and go along

    uint total_bags = get_number_of_bags_in(i_shinygold, bag_rules);

    cout << "total_bags: " << total_bags << endl;


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

        for (int j=1; j< smatchg.size(); j++){
            groups.push_back(smatchg[j].str());
        }

        matches.push_back({ smatchg.str(), groups });

    }
    return matches;
}

int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test.txt");
    auto slines = get_lines_from_file("input.txt");

    //parses lines

    // gets first part and adds to an unordered map that assigns an integer value to each bag
    unordered_map<string, uint> bags_to_uint;

    regex regex_pattern_bag("(.*) bags contain");
    smatch regex_match_str; //to collect regex match results
    
    uint ibag_str = 0;
    
    for (const auto& sline : slines) {
        cout << "sline: " << sline << endl;
        if (regex_search(sline, regex_match_str, regex_pattern_bag)) {
            //get the match group
            string match_str = regex_match_str[1].str();
            cout << "match_str: " << match_str <<endl;

            //Has this string already processed?
            if (bags_to_uint.find(match_str) != bags_to_uint.end()) {
                cout << "match already processed" << endl;
            }
            else{
                bags_to_uint[regex_match_str[1].str()] = ibag_str;
                cout << "match added to bags_to_uint map. ibag_str: " << ibag_str << endl;
                ibag_str++;
            }
        }
    }

    cout << "Print list of bags and integer tokens" << endl;
    for (const auto& kv : bags_to_uint) {
        cout << kv.first << " : " << kv.second << endl;
    }

    cout << "Get the bag rules" << endl;
    //set up bag rules
    //Extracts from the lines again
    list<Bag_rule> bag_rules;
    regex regex_pattern_bags_numbers("(\\d+) (\\w+ \\w+) bag"); // to match the rule information after
    smatch regex_match_str_bags_numbers; //to collect regex match results
    for (const auto& sline : slines) {
        cout << "sline: " << sline << endl;
        if (regex_search(sline, regex_match_str, regex_pattern_bag)) {
            //get the match group
            string match_str = regex_match_str[1].str();

            Bag_rule bag_rule0;
            uint i_prim = bags_to_uint[match_str];
            bag_rule0.i_primary_bag = i_prim;
            cout << "match_str: " << match_str << " , int: "<< i_prim << endl;

            //get and parse the rules
            // To find all matches, use the regex iterator
            //if (regex_search(sline, regex_match_str_bags_numbers, regex_pattern_bags_numbers) ){
            //    cout << "Match to bags info found" << endl;
            //    for (auto m : regex_match_str_bags_numbers) {
            //        cout << "match: " << m.str() << endl;
            //    }
            //    
            //}else {
            //    cout << "no other bags. No rule" << endl;
            //}
            //auto words_begin = sregex_iterator(sline.begin(), sline.end(), regex_pattern_bags_numbers);
            //auto words_end = sregex_iterator();

            //for (sregex_iterator i = words_begin; i != words_end; ++i) {
            //    smatch smatchg = *i;
            //    cout << "bags numbers match: " << smatchg.str(0) << std::endl;
            //    for (size_t j = 1; j < smatchg.size(); ++j) {
            //        std::cout << "bags numbers group " << j << ": " << smatchg.str(j) << std::endl;
            //    }
            //    if (smatchg.size() == 3) {
            //        //add info
            //        uint count = stoi(smatchg[1]);
            //        uint bag_idx0 = bags_to_uint[smatchg[2]];

            //        bag_rule0.contains.push_back({ count,bag_idx0 });
            //        cout << "Bag rule , i_prim: " << i_prim << " , count:" << count << " , bag_idx0:" << bag_idx0 << endl;

            //    }
            //}
            

            auto info_split = regex_get_matches_and_their_groups( sline, "(\\d+) (\\w+ \\w+) bag");
            // number , word word
            //This should get a match for each part of the contain section, and separated into groups.

            for (const auto& m0 : info_split) {
                //cout << "m0.first: " << m0.first << "  , m0.second[0]: " << m0.second[0] << "  , m0.second[1]: " << m0.second[1] << endl;
                uint count = stoi(m0.second[0]);
                uint bag_idx0 = bags_to_uint[m0.second[1]];
                bag_rule0.contains.push_back({ count,bag_idx0 });
                cout << "m0.first: " << m0.first << "  , m0.second[0]: " << m0.second[0] << "  , m0.second[1]: " << m0.second[1] << "bag_idx0:"<< bag_idx0 << endl;
            }

            bag_rules.push_back(bag_rule0);
        }

    }
    cout << "All rules processed" << endl;

    //part1(bag_rules, bags_to_uint); //ok
    part2(bag_rules, bags_to_uint); //ok

    return 0;
}