// https://adventofcode.com/2020/day/18

#include "../aoc_lmap_utils.h"
bool DEBUG = true;


uintll parse_string_token(string s) {

    uintll* expr_value_0 = nullptr;
    uintll* expr_value_1 = nullptr;
    string oper = "";

    //tuple<uintll*, string, uintll*> expression = { nullptr, "", nullptr };

    // Check if there is any parenthesis
    uintll par_count = 0;

    uintll i_start = -1;
    //uintll i_end = -1;

    bool is_number = false;
    for (uintll i=0; i <= s.size(); i++) {
        if (i == s.size() && is_number) {
            uintll res = stoi(s.substr(i_start, i - i_start));
            expr_value_1 = new uintll;
            *expr_value_1 = res;
            is_number = false;
        }
        else {
            auto c = s[i];

            if (par_count == 0) { //Only consider calculation if outside parenthisis
                if (isdigit(c)) {
                    if (!is_number) {
                        //number starts
                        i_start = i;
                        is_number = true;
                    }
                }
                else if (is_number) {
                    uintll res = stoi(s.substr(i_start, i - i_start));
                    expr_value_1 = new uintll;
                    *expr_value_1 = res;
                    is_number = false;
                }
                if (c == '+' || c == '*') {
                    oper = c;
                }
            }

            if (c == '(') {
                if (par_count == 0) {
                    i_start = i;
                }
                par_count += 1;
            }
            else if (c == ')') {
                if (par_count == 1) {
                    //auto i_end = i;
                    auto length = i - i_start;
                    //Need to calculate what is inside this token
                    auto inside_par_str = s.substr(i_start+1, length - 1);
                    auto value = parse_string_token(inside_par_str);
                    expr_value_1 = new uintll;
                    *expr_value_1 = value;
                }
                par_count -= 1;
            }

        }

        //Check if there is a full expression to evaluate
        if (expr_value_0 == nullptr && expr_value_1 != nullptr) {
            //uintll e0 = *expr_value_1;
            expr_value_0 = new uintll;
            *expr_value_0 = *expr_value_1;
            expr_value_1 = nullptr;
        }

        if (expr_value_0 != nullptr && expr_value_1 != nullptr && oper != "") {
            //Evaluate expression and reset
            
            //evaluate
            uintll n0 = *expr_value_0;
            uintll n1 = *expr_value_1;
            //uintll evaluation = 0; // for some reason this clears n0

            if (oper == "*") {
                n0 *= n1;
            }
            else if (oper == "+") {
                n0 += n1;
            }
            else {
                cerr << "Not valid expression" << endl;
            }
            *expr_value_0 = n0;
            expr_value_1 = nullptr;
            oper = "";
        }

    }

    uintll res = 0;
    if (expr_value_0 != nullptr) {
        res = *expr_value_0;
    }
    else {
        cerr << "Error parsing" << endl;
    }
    return res;

}


uintll parse_string_token2(string s) {

    vector<uintll> expressions;
    vector<char> operations;

    // Check if there is any parenthesis
    uintll par_count = 0;

    uintll i_start = -1;
    //uintll i_end = -1;

    bool is_number = false;

    for (uintll i = 0; i <= s.size(); i++) {
        if (i == s.size() && is_number) {
            //was last digit a number?
            uintll res = stoi(s.substr(i_start, i - i_start));
            expressions.push_back(res);
            is_number = false;
        }
        else {
            auto c = s[i];

            if (par_count == 0) { //Only consider calculation if outside parenthisis
                if (isdigit(c)) {
                    if (!is_number) {
                        //number starts
                        i_start = i;
                        is_number = true;
                    }
                }
                else if (is_number) {
                    uintll res = stoi(s.substr(i_start, i - i_start));
                    expressions.push_back(res);
                    is_number = false;
                }
                if (c == '+' || c == '*') {
                    operations.push_back(c);
                }
            }

            if (c == '(') {
                if (par_count == 0) {
                    i_start = i;
                }
                par_count += 1;
            }
            else if (c == ')') {
                if (par_count == 1) {
                    //auto i_end = i;
                    auto length = i - i_start;
                    //Need to calculate what is inside this token
                    auto inside_par_str = s.substr(i_start + 1, length - 1);
                    auto value = parse_string_token2(inside_par_str);
                    expressions.push_back(value);
                }
                par_count -= 1;
            }

        }
    }

    // Use expressions and operations to calculate final value
    // do addition operations first
    bool has_additions = true;
    while (has_additions) {
        has_additions = false;
        for (int i = 0; i < operations.size(); i++) {
            auto c = operations[i];
            if (c == '+') {
                has_additions = true;
                //do the addition
                auto res = expressions[i] + expressions[i + 1];
                expressions[i] = res; //replace
                operations.erase(operations.begin() + i);
                expressions.erase(expressions.begin() + i + 1);
                break;
            }
        }
    }

    //multiplication
    //cout << "multiplications" << endl;
    uintll mult = 1;
    for (const auto& exp0 : expressions) {

        mult *= exp0;
    }
    cout << "multiplication result is " << mult << endl;

    return mult;
}

void part1(vector<string> slines) {
    //TODO
    // split the line uintllo tokens
    // each token can be a number, operation or another token
    // Each token can contain other tokens

    uintll sum0 = 0;

    for (const auto& s : slines) {
        uintll res = parse_string_token(s);
        cout << s << " = " << res << endl;
        sum0 += res;

    }

    cout << "Sum of values is " << sum0 << endl;
}

void part2(vector<string> slines) {
    //Same as part 1 but prioritize additions

    uintll sum0 = 0;

    for (const auto& s : slines) {
        cout << s << endl;
        uintll res = parse_string_token2(s);
        cout << s << " = " << res << endl;
        sum0 += res;

    }

    cout << "Sum of values is " << sum0 << endl;
}

int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test.txt");
    auto slines = get_lines_from_file("input.txt"); DEBUG = false;
    
    //part1(slines);
    //Not 30754747866
    //202553439706 OK , requires long long precision

    part2(slines);
    //87606555779750 No
    // 88534268715686

    return 0;
}