// https://adventofcode.com/2020/day/21

#include "../aoc_lmap_utils.h"

bool TEST = false;
string test_file = "input_test.txt"; //9 tiles

unordered_set<string> ingredients;
unordered_set<string> allergens;

// map allergens to possible ingredients
// each allergen will be associated with a list of sets, each containing the ingredients list
// Its a bit like reversing the input
unordered_map <string, deque<unordered_set<string>>> allerg_ingredients_map;

unordered_map <string, uint> ingredient_count;

//unordered_map <string, string> allerg_ingred_assign;
// Make it a ordered map, to make it alphabeitcal to allergen
map <string, string> allerg_ingred_assign;

void parse_input_lines(strings_t slines) {
    // each line contains a list of ingredients, followed by a parenthesis containing the allergens

    for (const auto& sline : slines) {
        if (sline != "") {
            cout << "Line " << sline << endl;
            //crop substring until first parenthesis
            auto i = sline.find("(");
            if (i != sline.npos) {
                string left_side = sline.substr(0, i - 1);
                string right_side = sline.substr(i + 10, sline.size() - i - 11);

                cout << "left :" << left_side <<endl;
                cout << "right :" << right_side << endl;

                //Left side, split by space
                auto lefts = split(left_side, ' ');

                unordered_set<string> line_ingredients;
                for (auto l0 : lefts) {
                    ingredients.insert(l0);
                    line_ingredients.insert(l0);

                    ingredient_count[l0]+=1;
                }

                //Right side, split by comma, may contain spaces
                regex re(R"(\s*,\s*)"); // (space) comma (space)
                sregex_token_iterator it(right_side.begin(), right_side.end(), re, -1);
                sregex_token_iterator end;
                deque<string> allergens_tokenized(it, end); //

                //process each allergen
                for (const auto& allergen : allergens_tokenized) {
                    allergens.insert(allergen);

                    allerg_ingredients_map[allergen].push_back(line_ingredients);
                }
            }
        }
    }

    cout << "Data parsed. This is how it looks" << endl;

    for (const auto& m0 : allerg_ingredients_map) {
        cout << m0.first << " : ";

        for (const auto& set0 : m0.second) {
            for (const auto& i0 : set0) {
                cout << i0 << ", ";
            }
            cout << endl;
        }
        cout << endl;
    }
    cout << endl;

    cout << "Ingredients count" << endl;
    for (auto ing0 : ingredient_count) {
        cout << ing0.first << " : " << ing0.second << endl;
    }
    cout << endl;
    
}

void part1() {
    cout << "part1" << endl;

    //cycle to find solution.
    // Go through each allergen and intercept their own sets
    // If resulting set is empty then assign allergen to ingredient
    // And remove respective ingredient from the other sets

    unordered_map < string, unordered_set<string>> allerg_ingredients_inters; //intersection

    for (auto al_set : allerg_ingredients_map) {
        auto al = al_set.first;
        auto sets = al_set.second;

        if (sets.size() > 1) {
            //intercept them all
            unordered_set<string> intercept;
            auto set0 = sets[0];

            for (auto elem0 : sets[0]) {

                bool all_have_element = true;
                for (int i = 1; i < sets.size(); i++) {
                    auto set1 = sets[i];

                    if (set1.find(elem0) == set1.end()) {
                        all_have_element = false;
                        break;
                    }
                }

                if (all_have_element) {
                    intercept.insert(elem0);
                }
                allerg_ingredients_inters[al] = intercept;
            }
        }
        else {
            allerg_ingredients_inters[al] = *sets.begin();
        }
    }

    //if intercept has only one element, it becomes the allergen-ingredient assignment
    //unordered_map <string, string> allerg_ingred_assign;

    while (true) {
        for (auto el0 : allerg_ingredients_inters) {
            auto al0 = el0.first;
            if (allerg_ingred_assign.find(al0) == allerg_ingred_assign.end()) {
                //not assigned yet
                if (el0.second.size() == 1) {
                    auto ingred0 = *el0.second.begin();
                    //establish the assignment if not done yet
                    cout << "Establishing alergen " << al0 << " to " << ingred0 << endl;
                    allerg_ingred_assign[al0] = ingred0;
                }
            }

        }

        bool has_changed = false;

        //update allerg_ingredients_inters to remove assigned elements
        for (auto el0 : allerg_ingred_assign) {
            auto al_ass0 = el0.first;
            auto ingred0 = el0.second;

            for (auto& inters0_pair : allerg_ingredients_inters) {
                auto& inters0 = inters0_pair.second;
                if (inters0.find(ingred0) != inters0.end()) {
                    cout << "Clearing assigned alergen " << al_ass0 << " its ingredient " << ingred0 << " from intersection set" << endl;
                    inters0.erase(ingred0);

                    has_changed = true;
                }
            }

        }
        if (!has_changed) {
            break;
        }
    }
    cout << "No more changes to the sets" << endl;

    //Check which ingredients have not been assigned to an alergen
    unordered_set<string> ingredients_left(ingredients);

    for (const auto& el0 : allerg_ingred_assign) {
        auto ing0 = el0.second;
        ingredients_left.erase(ing0);
    }

    cout << "Non-allergen ingredients" << endl;
    uint solution_count = 0;
    for (auto ing0 : ingredients_left) {
        cout << ing0 << ", ";
        solution_count += ingredient_count[ing0];
    }
    cout << endl << "Part 1 solution: " << solution_count << endl;

}


void part2() {
    cout << "part2" << endl;

    string canonical_ing_list = "";

    cout << "Assigned alergens" << endl;
    for (const auto& el0 : allerg_ingred_assign) {
        auto al0 = el0.first;
        auto ing0 = el0.second;

        cout << al0 << ",";
        canonical_ing_list += ing0 + ",";
    }

    cout << endl << "canonical" << endl;

    canonical_ing_list = canonical_ing_list.substr(0, canonical_ing_list.size() - 1);
    cout << canonical_ing_list << endl;

}

int main() {
    //Read data from file to list of strings

    string input_file = "input.txt";
    if (TEST) {
        input_file = test_file;
    }
    auto slines = get_lines_from_file(input_file.c_str());

    parse_input_lines(slines);

    part1();


    //Use last solution to work out part2
    part2();


    return 0;
}