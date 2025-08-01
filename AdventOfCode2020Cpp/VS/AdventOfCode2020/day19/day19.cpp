// https://adventofcode.com/2020/day/19

/* TODO
* 
* The solution that is currently being explored will not work
* the number of possible combinations even for the first rule is too large
* to list all rules possible
* Consider a solution that is recursive and deals with each input individually.
* 
* */


#include "../aoc_lmap_utils.h"
#include <regex>
bool TEST = true;

//string test_file = "input_test_part2_singlemsg.txt";
//string test_file = "input_test_part2_special1.txt";
//string test_file = "input_test_part2.txt"; //answer should be 12, ok
string test_file = "input.txt";

//unordered_map<uint, string> rules_map;
vector<string> m_messages;

using Rules = deque<string>;
unordered_map<uint, Rules> rules_resolved;

using SubRule = deque<uint>;
class cRule;

unordered_map<uint, cRule> cRulesMap;


string get_text_from_dqints(const deque<uint> dquints) {
    string s = "";
    
    for (const auto& i0 : dquints) {
        s += to_string(i0) + ", ";
    }
    return s;

}
class cRule
{
//private:

public:
    bool _isSingleChar;
    char _rulechar;
    // SubRule _sub_rule;
    deque<SubRule> _sub_rule_choices;
    cRule()
    {
        //parse whole rule to internal representation

        _isSingleChar = false;
        _rulechar = '0';
    }

    static cRule fromRuleLine(string sruleline)
    {
        //check is quoted string
        if (int spos0 = sruleline.find("\"") != sruleline.npos) {
            int spos1 = sruleline.find("\"", spos0 + 1);

            if (spos1 = spos0 + 2) {
                char char0;
                char0 = sruleline.substr(spos0 + 1, 1).c_str()[0];

                cout << "Contains CHAR " << char0 << endl;

                cRule cRule0;
                cRule0._isSingleChar = true;
                cRule0._rulechar = char0;

                return cRule0;
            }
        }
        else {
            deque<string> sub_rule_parts; //to seperate subrules
            //contains pipe?
            size_t pipe_pos = sruleline.find('|');
            if (pipe_pos != sruleline.npos) {
                sub_rule_parts.push_back(sruleline.substr(0, pipe_pos - 1));
                sub_rule_parts.push_back(sruleline.substr(pipe_pos + 1, sruleline.size() - pipe_pos));
            }
            else {
                sub_rule_parts.push_back(sruleline);
            }

            if (sub_rule_parts.size() > 0) {

                cRule cRule0;
                cRule0._isSingleChar = false;
                //cRule0._rulechar = '0';

                //parse the subrules strings
                for (const auto& subrulepart0 : sub_rule_parts) {
                    //split rule to its decimal numbers split by space

                    cout << "subrule part: " << subrulepart0 << endl;

                    //Use regex to iterate through each decimal number
                    regex regex_rule_pattern("\\d+");
                    auto words_begin = std::sregex_iterator(subrulepart0.begin(), subrulepart0.end(), regex_rule_pattern);
                    auto words_end = std::sregex_iterator();
                    if (words_begin != words_end) {
                        SubRule subrule0;

                        for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
                            std::smatch match = *it;

                            uint id_new = std::stoul(match.str()); //convert string to decimal uint

                            subrule0.push_back(id_new);
                        }
                        cRule0._sub_rule_choices.push_back(subrule0);
                    }
                }
                return cRule0;
            }
        }

        //Need to return someting, even none?
        cerr << "Could not parse rule line" << endl;
    }
};

//Rules get_rules_from_id(const uint rule_id) {
//    cout << "get_rules_from_id " << rule_id << endl;
//
//    // Gets rule(s) and if not resolved it will resolve it
//    Rules rules_res;
//
//    if (rules_resolved.find(rule_id) == rules_resolved.end()) {
//        // rule not sorted yet
//
//        Rules rule0;
//
//
//        //parse information from the unparsed rules list
//
//        string srule0 = rules_map[rule_id];
//        //string can either be a letter inside quotes
//        // or numbers with a pipe
//
//        cout << "Processing rule with id " << rule_id << " containing string " << srule0 << endl;
//
//        //check is quoted string
//        if (int spos0 = srule0.find("\"") != srule0.npos) {
//            int spos1 = srule0.find("\"", spos0 + 1);
//
//            if (spos1 = spos0 + 2) {
//                string char0 = srule0.substr(spos0 + 1,1);
//                cout << "Contains CHAR " << char0 << endl;
//
//                Rules r0 = { char0 };
//
//                rules_res = r0;
//            }
//        }
//        else {
//            //not a quoted string, will contain numbers
//            
//            deque<string> rule_parts;
//
//            //contains pipe?
//            size_t pipe_pos = srule0.find('|');
//            if (pipe_pos != srule0.npos) {
//                rule_parts.push_back(srule0.substr(0, pipe_pos - 1));
//                rule_parts.push_back(srule0.substr(pipe_pos+1, srule0.size() - pipe_pos));
//            }
//            else {
//                rule_parts.push_back(srule0);
//            }
//
//            if (rule_parts.size() > 0) {
//
//                string rule_string = "";
//
//                //Rules mix_rules;
//
//                for (const auto& rulepart0 : rule_parts) {
//                    //split rule to its decimal numbers split by space
//
//                    cout << "part: " << rulepart0 << endl;
//
//                    regex regex_rule_pattern("\\d+");
//
//                    auto words_begin = std::sregex_iterator(rulepart0.begin(), rulepart0.end(), regex_rule_pattern);
//                    auto words_end = std::sregex_iterator();
//
//                    deque<Rules> rules_in_ids;
//
//                    //std::cout << "Matches found:\n";
//                    for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
//                        std::smatch match = *it;
//
//                        uint id_new = std::stoul(match.str());
//
//                        Rules rule_in_id = get_rules_from_id(id_new);
//                        rules_in_ids.push_back(rule_in_id);
//                    }
//
//                    if (rules_in_ids.size() == 1) {
//                        //Simply return the rule that was received
//                        rules_res = rules_in_ids[0] ;
//                    }
//                    else if (rules_in_ids.size() == 2) {
//
//                        for (const auto& rule0 : rules_in_ids[0]) {
//                            for (const auto& rule1 : rules_in_ids[1]) {
//
//                                string s = rule0 + rule1;
//
//                                rules_res.push_back(s);
//                            }
//                        }
//                    }
//                    else if (rules_in_ids.size() == 3) {
//
//                        for (const auto& rule0 : rules_in_ids[0]) {
//                            for (const auto& rule1 : rules_in_ids[1]) {
//                                for (const auto& rule2 : rules_in_ids[2]) {
//
//                                    string s = rule0 + rule1 + rule2;
//
//                                    rules_res.push_back(s);
//                                }
//                            }
//                        }
//                    }
//                    else {
//                        cerr << "Too many rules " << rules_in_ids.size() << endl;
//                    }
//                }
//            }
//        }
//        rules_resolved[rule_id] = rules_res;
//        cout << "Just resolved rule(s) from id " << rule_id << " has size " << rules_res.size() << endl;
//    }
//    else {
//        // rule already resolved, so just reuse it
//        rules_res = rules_resolved[rule_id];
//    }
//
//    return rules_res;
//
//}


Rules get_rules_from_id(const uint rule_id) {
    cout << "get_rules_from_id " << rule_id << endl;

    // Gets rule(s) and if not resolved it will resolve it
    Rules rules_res;

    if (rules_resolved.find(rule_id) == rules_resolved.end()) {
        // rule not sorted yet

        Rules rule0;

        auto crule0 = cRulesMap[rule_id];

        if (crule0._isSingleChar) {
            Rules r0 = { string(1,crule0._rulechar) };
            
            rules_res = r0;
        }

        

        if (crule0._sub_rule_choices.size() > 0) {
            string rule_string = "";
            
            for (const auto& crulepart0 : crule0._sub_rule_choices) {
                //split rule to its decimal numbers split by space

                //cout << "part: " << crulepart0. << endl;
                deque<Rules> rules_in_ids;
                for (const auto& subrule0 : crulepart0) {

                    Rules rule_in_id = get_rules_from_id(subrule0);
                    rules_in_ids.push_back(rule_in_id);
                }
                if (rules_in_ids.size() == 1) {
                    //Simply return the rule(s) that was received
                    //rules_res = rules_in_ids[0];
                    for (const auto& rule0 : rules_in_ids[0]) {
                        rules_res.push_back(rule0);
                    }
                }
                else if (rules_in_ids.size() == 2) {

                    for (const auto& rule0 : rules_in_ids[0]) {
                        for (const auto& rule1 : rules_in_ids[1]) {

                            string s = rule0 + rule1;

                            rules_res.push_back(s);
                        }
                    }
                }
                else if (rules_in_ids.size() == 3) {

                    for (const auto& rule0 : rules_in_ids[0]) {
                        for (const auto& rule1 : rules_in_ids[1]) {
                            for (const auto& rule2 : rules_in_ids[2]) {

                                string s = rule0 + rule1 + rule2;

                                rules_res.push_back(s);
                            }
                        }
                    }
                }
                else {
                    cerr << "Too many rules " << rules_in_ids.size() << endl;
                }
            }
        }
        rules_resolved[rule_id] = rules_res;
        cout << "Just resolved rule(s) from id " << rule_id << " has size " << rules_res.size() << endl;
    }else{
        // rule already resolved, so just reuse it
        rules_res = rules_resolved[rule_id];
    }

    return rules_res;

}

//void resolve_rules() {
//    //go through each of the rule one by one
//    // If it is a quoted string, keep it and go to next one
//    // Otherwise goes deeper
//
//    for (const auto& rule0 : rules_map) {
//        uint rule_id = rule0.first;
//        string rule_desc = rule0.second;
//
//        Rules r = get_rules_from_id(rule_id);
//
//
//    }
//
//}

void is_message_fully_valid_for_rule_id(const string message, const uint rule_id,
    bool& is_valid_out,
    string& remaining_message
    ){

    // This is meant to be used recursively to check if message is valid
    // It returns result via function parameters
    // similar to DFS

    // TODO: FIX
    // It is possible that the message has choices, and is ok for the first part but fails the second part
    // which then fails completely

    //It should explore all possibilities so it cannot be simply recursive.

    cout << "is_message_fully_valid_for_rule_id() ; " <<
        "message: " << message << ", ruleid: " << rule_id << endl;

    if (message.size() > 0) {

        // gets the instance of the rule
        cRule crule0 = cRulesMap[rule_id];

        if (crule0._isSingleChar) {
            char c = message[0];
            //Check against first character
            if (crule0._rulechar == c) {
                cout << "Single char " << c << " is ok" <<endl;
                is_valid_out = true;
                remaining_message = message.substr(1, message.size() - 1);
                return;
            } {
                cout << "Single char " << c << " is no match to " << crule0._rulechar << endl;
            }
        }
        else {
            //Check list inside the rule
            auto choices = crule0._sub_rule_choices;

            is_valid_out = false;
            //Try to find a rule that succeeds
            /*auto it = choices.begin();
            while (!is_valid_out) {*/

            for (auto it=choices.begin(); it != choices.end(); it++){
                
                auto subrule = *it;

                // The subrule is a list of rules, all must be valid
                if (subrule.size() <= 0) {
                    cerr << "subrule size: " << subrule.size() << " is too small" << endl;
                }
                bool whole_subrule_valid = true; //defaults to true;
                bool is_message_valid_ret;
                string remain_message_ret(message);

                for (const auto& id0 : subrule) {

                    //Call the given id
                    is_message_fully_valid_for_rule_id(remain_message_ret, id0,
                        is_message_valid_ret,
                        remain_message_ret);

                    if (!is_message_valid_ret) {
                        whole_subrule_valid = false;
                        break; //exit for loop
                    }
                }

                if (whole_subrule_valid) {
                    //subrule valid
                    is_valid_out = true;
                    remaining_message = remain_message_ret;

                    return;
                    //It should return a list
                }
                //else
                // check next subrule
                //it++;
            }
        }
    }
    is_valid_out = false;
    remaining_message = "";

    return;
}

//Just declare
void is_message_fully_valid_for_rule_id_2(const string message, const uint rule_id,
    bool& is_valid_out,
    deque<string>& remaining_valid_messages
);

void is_message_valid_for_subrule(const string message, SubRule subrule,
    bool& is_valid_out,
    string remaining_valid_message
) {
    // All rules must be verified
    bool whole_subrule_valid = true; //defaults to true;
    bool is_message_valid_ret;
    
    deque<string> remain_messages_ret;
    remain_messages_ret.push_back(message);

    string last_valid_rem_string;

    string msg0;

    for (const auto& id0 : subrule) {
        bool b_id_valid = false;

        deque<string> new_remain_msgs;

        for (auto it_msg0 = remain_messages_ret.begin(); it_msg0 != remain_messages_ret.end();  it_msg0++) {
            msg0 = *it_msg0;

            //Call the given id
            deque<string> remain_messages_ret0;
            is_message_fully_valid_for_rule_id_2(
                msg0,
                id0,
                is_message_valid_ret,
                remain_messages_ret0);
            if (is_message_valid_ret) {
                //basically appends one deque to another
                new_remain_msgs.insert(new_remain_msgs.end(), remain_messages_ret0.begin(), remain_messages_ret0.end());
                
                b_id_valid |= is_message_valid_ret;
            }
        }
        remain_messages_ret = new_remain_msgs;

        whole_subrule_valid &= b_id_valid;
    }
    if (whole_subrule_valid) {
        remaining_valid_message = msg0;
        is_valid_out = true;
    }
}

void is_message_fully_valid_for_rule_id_2(const string message, const uint rule_id,
    bool& is_valid_out,
    deque<string>& remaining_valid_messages
) {

    //

    cout << "is_message_fully_valid_for_rule_id() ; " <<
        "message: " << message << ", ruleid: " << rule_id << endl;

    if (message.size() > 0) {

        remaining_valid_messages.clear();

        // gets the instance of the rule
        cRule crule0 = cRulesMap[rule_id];

        if (crule0._isSingleChar) {
            char c = message[0];
            //Check against first character
            if (crule0._rulechar == c) {
                cout << "Single char " << c << " is ok" << endl;
                is_valid_out = true;
                string remaining_message = message.substr(1, message.size() - 1);
                remaining_valid_messages.push_back(remaining_message);
                return;
            } {
                cout << "Single char " << c << " is no match to " << crule0._rulechar << endl;
            }
        }
        else {
            //Check list inside the rule


            auto sub_rule_choices = crule0._sub_rule_choices;

            is_valid_out = false;
            //Try to find a rule that succeeds
            /*auto it = choices.begin();
            while (!is_valid_out) {*/

            string remain_msg_ret;
            bool is_valid;

            if (sub_rule_choices.size() == 1) {
                auto subrule0 = sub_rule_choices[0];

                is_message_valid_for_subrule(message, subrule0, is_valid, remain_msg_ret);
                remaining_valid_messages.push_back(remain_msg_ret);

            }
            else if (sub_rule_choices.size() == 2) {
                auto subrule0 = sub_rule_choices[0];
                auto subrule1 = sub_rule_choices[1];

                is_message_valid_for_subrule(message, subrule0, is_valid, remain_msg_ret);
                if (is_valid) {
                    remaining_valid_messages.push_back(remain_msg_ret);
                    is_valid_out = true;
                }

                is_message_valid_for_subrule(message, subrule1, is_valid, remain_msg_ret);
                if (is_valid) {
                    remaining_valid_messages.push_back(remain_msg_ret);
                    is_valid_out = true;
                }
            }
        }
    }
}


//Consider using a cache
//Try another strategy, DFS, potentially mixed
bool part2_check_subrule_ids_dfs(deque<uint> ids, string text) {
    //This version does not return remaining text

    cout << "part2_check_subrule_ids(), " << get_text_from_dqints(ids) << "; Text " << text << endl;

    if (ids.size() > 0) {

        deque<uint> m_ids(ids); //clones original

        // Check firs id
        auto id0 = m_ids.front();
        m_ids.pop_front();
        //cout << "rule id " << id0 << endl;

        auto crule0 = cRulesMap[id0];

        // Analyse the rule against message
        if (crule0._isSingleChar) {
            //Check against first character of text
            char c = text[0];
            if (crule0._rulechar == c) {
                cout << "Single char " << c << " is ok" << endl;

                auto remaining_text = text.substr(1, text.size() - 1);

                if (remaining_text.size() > 0) {
                    //Goes DFS deeper
                    return part2_check_subrule_ids_dfs(m_ids, remaining_text);
                }
                else {
                    if (m_ids.size() == 0) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }

            } {
                cout << "Single char " << c << " is no match to " << crule0._rulechar << endl;
                return false;
            }
        }
        else {
            //Check list inside the rule
            auto subrules = crule0._sub_rule_choices;

            cout << "Rule id " << id0 << " has " << subrules.size() << " subrules" << endl;

            //for (auto it = choices.begin(); it != choices.end(); it++) {
            //    auto subrule = *it;
            // 
            // If any subrule is ok, then return true
            bool any_subrule_valid = false;

            auto m_subrules(subrules);

            for (const auto& subrule0 : subrules) {
                cout << "id "<< id0 << " . Checking subrule " << get_text_from_dqints(subrule0) << endl;

                //bool res = part2_check_subrule_ids(subrule0, text);
                //TODO: construct subrule that appends subrule to the existing rules at m_ids
                
                auto ids_prog(subrule0); //clones
                ids_prog.insert(ids_prog.end(), m_ids.begin(), m_ids.end()); //concatenate

                bool res = part2_check_subrule_ids_dfs(ids_prog, text);

                if (res == true) {
                    cout << "id "<< id0 << ". Subrule ok for text " << text << ". Returning true." << endl;
                    return true;
                }
                else {
                    cout << "id " << id0 << ". Subrule failed for text " << text << ". Try next one" << endl;
                }
            }
        }
    }
    //If reached here is because none of the subrules is valid
    cout << "Check failed for " << get_text_from_dqints(ids) << "; Text " << text << endl;
    return false;
}


void part1() {
    cout << "part1" << endl;
    // Sort rules?
    //resolve_rules();
    //get_rules_from_id(0);

    //cout << "All rules in char format" << endl;

    //for (const auto& r0 : rules_resolved) {
    //    cout << "rule id : " << r0.first << endl;
    //    for (const auto& s0 : r0.second) {
    //        cout << s0 << endl;
    //    }
    //}

    // Go through each message and check if it is valid against a rule
    //Debug: check first message only against first rule
    
    uint nmatches = 0;

    //auto messg0 = m_messages[0];
    //bool is_valid_out = false;
    //string message_remaining_out = "";

    //is_message_fully_valid_for_rule_id(messg0, 0, is_valid_out, message_remaining_out);

    for (const auto& messg0 : m_messages) {
        cout << "MESSAGE :" << messg0 << endl;

        bool is_valid_out = false;
        string message_remaining_out = "";
        is_message_fully_valid_for_rule_id(messg0, 0, is_valid_out, message_remaining_out);

        cout << "RESULT: is_valid_out: " << is_valid_out << ", message_remaining_out:" << message_remaining_out << endl;

        if (is_valid_out && message_remaining_out.size() == 0) {
            nmatches++;
            cout << "MATCH. nmatches: " << nmatches <<endl;
        }
    }
    cout << "Final nmatches: " << nmatches << endl;

    //OK, but too much debugging text

}


void part2_depth() {
    //NOT WORKING
    cout << "part2_depth" << endl;

    uint nmatches = 0;

    for (const auto& messg0 : m_messages) {
        cout << "MESSAGE :" << messg0 << endl;

        bool is_valid_out = false;
        deque<string> messages_remaining_out;
        is_message_fully_valid_for_rule_id_2(messg0, 0, is_valid_out, messages_remaining_out);

        //cout << "RESULT: is_valid_out: " << is_valid_out << ", message_remaining_out:" << message_remaining_out << endl;

        if (is_valid_out && messages_remaining_out.size() == 0) {
            nmatches++;
            cout << "MATCH. nmatches: " << nmatches << endl;
        }
    }
    cout << "Final nmatches: " << nmatches << endl;


}


void part2_dfs() {
    cout << "part2_dfs" << endl;

    uint nmatches = 0;

    deque<string> matches;

    for (const auto& messg0 : m_messages) {
        cout << "MESSAGE :" << messg0 << endl;

        deque<uint> startid = { 0 };

        bool res = part2_check_subrule_ids_dfs(startid, messg0);

        cout << "RESULT: " << res << endl;

        if (res) {
            nmatches++;
            cout << "MATCH. nmatches: " << nmatches << endl;
            matches.push_back(messg0);
        }
    }
    cout << "Final nmatches: " << nmatches << endl;

    for (const auto& s : matches) {
        cout << s << endl;
    }

    // aaaabbaaaabbaaa is showing a match, and it should not

}


void part2() {

    //Some rules change
    /*
    * replace rules 8: 42 and 11: 42 31 with the following:

8: 42 | 42 8
11: 42 31 | 42 11 31
    * */

    //Problem suggests to have a look at what happens to these cyclic rules.

    // Rule 8 suggests that there can be a match if string(s) corresponding to 42 is repeated.
    // In normal operation, it would check for strings42, and if there is a match, returns ok.
    // However it may fail in the next rule. But the rule 8 says that repetitions of 42 are acceptable.
    // 
    // It is probably best to check for the repetition first, maybe swapping the order of subrules

    // auto cr8 = cRule::fromRuleLine("42 8 | 42 ");
    auto cr8 = cRule::fromRuleLine("42 | 42 8");
    cRulesMap[8] = cr8;

    //auto cr11 = cRule::fromRuleLine("42 11 31 | 42 31");
    auto cr11 = cRule::fromRuleLine("42 31 | 42 11 31");
    cRulesMap[11] = cr11;

    //cout << "get_rules_from_id(42)" << endl;
    get_rules_from_id(42);

    cout << "Rule 42, all combinations" << endl;
    for (const auto& str_ruleres0 : rules_resolved[42]) {
        cout << str_ruleres0 << endl;
    }

    //cout << "get_rules_from_id(42)" << endl;
    get_rules_from_id(31);

    cout << "Rule 31, all combinations" << endl;
    for (const auto& str_ruleres0 : rules_resolved[31]) {
        cout << str_ruleres0 << endl;
    }

    cout << endl;
    cout << "Rules resolved";
    //Rules resolved
    for (const auto& pair : rules_resolved) {
        uint id0 = pair.first;
        cout << endl << "Rule " << id0 << endl;
        for (const auto& str_ruleres0 : rules_resolved[id0]) {
            cout << str_ruleres0 << " ; ";
        }
    }

    // 
    part2_dfs();

}


int main() {
    //Read data from file to list of strings

    string input_file = "input.txt";
    if (TEST) {
        input_file = test_file;
    }
    auto slines = get_lines_from_file(input_file.c_str());

    // TODO: seperate rules from messages
    // Use regex to distinguish what is rule and what is message

    regex regex_rule_pattern("(\\d+):(.+)");
    smatch regex_match; //to collect regex match results

    for (const auto& s : slines) {
        if (s != "") {
            //Check against rule pattern

            if (regex_search(s, regex_match, regex_rule_pattern)) {
                if (regex_match.size() != 3) {
                    cerr << "Error with string: " << s << " . Number of matches is "
                        << regex_match.size() << " and it should be 3." << endl;
                }
                else {
                    uint i0 = stoul(regex_match[1]);
                    string srule = regex_match[2];
                    //rules_map[i0] = srule;

                    cRule c0 = cRule::fromRuleLine(srule);
                    cRulesMap[i0] = c0;
                }
            }
            else {
                //assume it is a message
                m_messages.push_back(s);
            }
        }
    }

    cout << "Number of rules: " << cRulesMap.size() << endl;
    cout << "Number of messages: " << m_messages.size() << endl;

    //part1();

    part2();

    
    return 0;
}