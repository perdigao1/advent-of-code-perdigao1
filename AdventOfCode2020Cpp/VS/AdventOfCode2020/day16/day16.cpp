// https://adventofcode.com/2020/day/16

#include "../aoc_lmap_utils.h"
#include <unordered_set>

using namespace std;

struct TicketRule {
    string name;
    pair < pair<uint,uint>, pair<uint,uint> > ranges;
};

auto parse_file(const vector<string>& slines) {
    
    enum readtype { unset, rules, your_ticket, nearby_tickets };

    readtype readtype0 = rules;

    vector<TicketRule> ticket_rules0;
    vector<uint> your_ticket0;
    vector<vector<uint>> nearby_tickets0;

    //string pat_rules = "(\\w+): (\\d*)-(\\d*) or (\\d*)-(\\d*)";
    string pat_rules = "(.+): (\\d*)-(\\d*) or (\\d*)-(\\d*)";

    for (const auto& s0 : slines) {

        switch (readtype0) {
            case unset:
                //looking for either "your ticket" or "nearby tickets"
                if (s0.find("your ticket") != s0.npos) {
                    readtype0 = your_ticket;
                    continue;
                }
                else if(s0.find("nearby tickets") != s0.npos) {
                    readtype0 = nearby_tickets;
                }
                break;
            case rules: {
                if (s0.length() > 0) {
                    auto regex_res = regex_get_matches_and_their_groups(s0, pat_rules);

                    if (regex_res.size() > 0) {
                        auto res_groups = regex_res[0].groups;
                        //parse rule
                        //TODO check size=5 is ok
                        auto rule_str = res_groups[0];

                        pair<uint, uint> rule0 = { stoi(res_groups[1]) , stoi(res_groups[2]) };
                        pair<uint, uint> rule1 = { stoi(res_groups[3]) , stoi(res_groups[4]) };

                        ticket_rules0.push_back(TicketRule{ rule_str,{ rule0, rule1 } });
                    }
                    else {
                        readtype0 = unset;
                    }
                }
                else {
                    readtype0 = unset;
                }
                break;
            }
            case your_ticket: {
                auto tick_numbers_str = split(s0, ',');

                if (tick_numbers_str.size() > 0) {
                    for (const auto& tn0 : tick_numbers_str) {
                        your_ticket0.push_back(stoi(tn0));
                    }
                }
                readtype0 = unset;
                break;
            }
            case nearby_tickets: {
                vector<uint> ticket0;
                auto tick_numbers_str0 = split(s0, ',');
                if (tick_numbers_str0.size() > 0) {
                    for (const auto& tn0 : tick_numbers_str0) {
                        ticket0.push_back(stoi(tn0));
                    }
                }
                nearby_tickets0.push_back(ticket0);
                break;
            }
        }
    }



    struct Result {
        vector<TicketRule> rules;
        vector<uint> your_ticket;
        vector<vector<uint>> nearby_tickets;
    };

    return Result{
        //return values here
        ticket_rules0,
        your_ticket0,
        nearby_tickets0
    };
    
}

void part1(vector<string> slines) {
    // Start by collecting ranges
    // Store ranges in list of pairs

    auto res = parse_file(slines);

    cout << "File parsed " << endl;

    // Check for validity of nearby tickets and count them

    uint count_inv_nb_tickets = 0;

    for (const auto& nbs0 : res.nearby_tickets) {
        for (const auto& nb0 : nbs0) {
            // check against each interaval
            bool is_inside = false;
            for (const auto& r0 : res.rules) {
                auto ranges0 = r0.ranges;
                if ((nb0 >= ranges0.first.first && nb0 <= ranges0.first.second) ||
                    (nb0 >= ranges0.second.first && nb0 <= ranges0.second.second)) {
                    is_inside = true;
                    break;
                }
            }
            if (!is_inside) {
                cout << "Ticket " << nb0 << " outside all ranges" << endl;
                count_inv_nb_tickets+=nb0;
            }
        }
    }

    cout << "Scanning error rate: " << count_inv_nb_tickets << endl;
}


void part2(vector<string> slines) {
    // Start by collecting ranges
    // Store ranges in list of pairs

    auto res = parse_file(slines);

    cout << "Part2. File parsed " << endl;

    //Identify tickets with invalid values and discard (or not consider them)

    vector<vector<uint>> valid_nearby_tickets;

    for (const auto& nbs0 : res.nearby_tickets) {
        bool ticket_is_valid = true;

        for (const auto& nb0 : nbs0) {
            // check against each interval
            bool value_is_valid = false;
            for (const auto& r0 : res.rules) {
                auto ranges0 = r0.ranges;
                if ((nb0 >= ranges0.first.first && nb0 <= ranges0.first.second) ||
                    (nb0 >= ranges0.second.first && nb0 <= ranges0.second.second)) {
                    value_is_valid = true;
                    break;
                }
 
            }
            if (!value_is_valid) {
                ticket_is_valid = false;
                break;
            }
        }
        if (ticket_is_valid) {
            valid_nearby_tickets.push_back(nbs0);
        }
    }

    cout << "valid nearby tickets are" << endl;
    for (const auto& nbs0 : valid_nearby_tickets) {
        for (const auto& nb0 : nbs0) {
            cout << nb0 << ", ";
        }
        cout << endl;
    }

    // Need to match fields with rules
    // Start with a lost of all rules for each field, represented with an int.
    // If range lies outside, then remove respective entry

    vector<unordered_set<uint>> match0; //first index is position number
    for (int i = 0; i < res.rules.size(); i++) {
        unordered_set<uint> set0;
        for (int j = 0; j < res.rules.size(); j++) {
            set0.insert(j);
        }
        match0.push_back(set0);
    }

    // Now, go through all values and check if need to remove
    // Similar code to the one used to chcck if ticket was valid
    for (const auto& ticket0 : valid_nearby_tickets) {
        for (uint i = 0; i < ticket0.size(); i++) {
            auto tick_entry = ticket0[i];

            //Chack against each of the rules
            for (uint irule = 0; irule < res.rules.size(); irule++) {
                auto ranges0 = res.rules[irule].ranges;

                if (!((tick_entry >= ranges0.first.first && tick_entry <= ranges0.first.second) ||
                    (tick_entry >= ranges0.second.first && tick_entry <= ranges0.second.second))) {
                    //range is not valid in this rule, remove entry
                    match0[i].erase(irule);
                }
            }
        }
    }
    
    // Print matches
    cout << "matches0" << endl;
    for (uint ipos = 0; ipos < match0.size(); ipos++) {
        auto m0 = match0[ipos];
        for (const auto& irule : m0) {
            cout << "ipos " << ipos << " irule " << irule << endl;
        }
    }
    
    //There are positions that can match with several rules.
    // Ensure one-to-one
    // Run in a cycle until no changes
    // For each position check number of rules matching.
    // If one, put in list
    // If more than one, check each of them against already established set and remove entry
    // Repeat until no change
    cout << "Revising" << endl;

    bool has_changed = false;

    unordered_set<uint> irule_established;
    do {
        has_changed = false;
        for (uint ipos = 0; ipos < match0.size(); ipos++) {
            
            auto m0 = match0[ipos];

            //Check number of positions
            if (m0.size() == 1) {
                auto irule0 = *m0.begin();
                if (irule_established.find(irule0) == irule_established.end()) {
                    irule_established.insert(*m0.begin());
                    has_changed = true;
                }
            }
            else {
                //Check against irule_established
                for (const auto& irule : m0) {
                    if (irule_established.find(irule) != irule_established.end()) {
                        match0[ipos].erase(irule);
                        has_changed = true;
                    }
                }
            }
        }
    } while (has_changed);

    cout << "Check complete" << endl;

    cout << "matches0 revised" << endl; 
    unordered_map<uint, uint> irule_to_ipos; //Reverse to get irule to ipos
    for (uint ipos = 0; ipos < match0.size(); ipos++) {
        auto m0 = match0[ipos];
        for (const auto& irule : m0) {
            cout << "ipos " << ipos << " irule " << irule << endl;
            irule_to_ipos[irule] = ipos;
        }
    }

    cout << "Calculating dep_mult" << endl;
    //TODO: Complete the rest
    //Check which rules indices start with the word "departure"
    uintll dep_mult = 1;
    for (uint irule = 0; irule < res.rules.size(); irule++) {
        auto rule0 = res.rules[irule];
        //departure
        if (rule0.name.substr(0, 9) == "departure") {
            //use this index to get value in my ticket
            
            auto ipos = irule_to_ipos[irule];
            auto v = res.your_ticket[ipos];

            cout << "rule "<< irule << " ipos "<< ipos<< ", ticket value is " << v<<endl;
            dep_mult *= v;
        }
    }

    cout << "Result is " << dep_mult;
}

int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test.txt"); //71 ok
    //auto slines = get_lines_from_file("input.txt"); // 19087, ok
    //part1(slines);

    //auto slines = get_lines_from_file("input_test.txt");
    //auto slines = get_lines_from_file("input_test2.txt");
    //auto slines = get_lines_from_file("input_test3.txt");
    auto slines = get_lines_from_file("input.txt"); // 19087, ok
    part2(slines);


    return 0;
}