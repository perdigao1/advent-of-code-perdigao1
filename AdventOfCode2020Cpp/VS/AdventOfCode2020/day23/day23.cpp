// https://adventofcode.com/2020/day/23

#include "../aoc_lmap_utils.h"

using game_cups = deque<uint>;

void print_cups(game_cups cups) {
    for (const auto& c0 : cups) {
        cout << c0 << ",";
    }
}

void part1(game_cups cups, const uint nmoves) {
    cout << "part1" << endl;

    cout << "Game cups are ";
    uint cup_min = cups[0]; uint cup_max = cups[0];
    for (const auto& c0 : cups) {
        cout << c0 << ",";
        cup_min = min(c0, cup_min);
        cup_max = max(c0, cup_max);
    }
    cout << endl;
    cout << "cup_min " << cup_min << " cup_max " << cup_max << endl;

    //const int nmoves = 100;

    auto current_cup_it = cups.begin() + 0 ; //index 0

    for (int i = 0; i < nmoves; i++) {

        //picks 3 cups next to current cup
        // cups are removed and spacing is maintained

        cout << "move " << i << " ; cups ";
        print_cups(cups);
        cout << endl;

        auto current_cup_value = *current_cup_it;

        game_cups picked;
        for (int ipick=0; ipick < 3; ipick++) {
            auto pick_it = current_cup_it + 1;
            if (pick_it == cups.end()) pick_it = cups.begin();

            auto pick = *pick_it;
            cups.erase(pick_it);
            picked.push_back(pick);

            //Maintain reference
            current_cup_it = find(cups.begin(), cups.end(), current_cup_value);
        }
        
        cout << "picked "; print_cups(picked); cout << endl;

        int dest_cup_value = current_cup_value - 1;
        cout << "dest cup " << dest_cup_value <<endl;
        
        
        while (find(picked.begin(), picked.end(), dest_cup_value) != picked.end() ||
            find(cups.begin(), cups.end(), dest_cup_value)==cups.end()) {
            dest_cup_value-=1;
            if (dest_cup_value < (int)cup_min) {
                dest_cup_value = cup_max;
            }
        }
        
        //destination cup location, from iterators
        auto dest_loc_it = find(cups.begin(), cups.end(), dest_cup_value)+1;
        
        cups.insert(dest_loc_it, picked.begin(), picked.end());
        
        // Find iterator of current cup, and set the next
        current_cup_it = find(cups.begin(), cups.end(), current_cup_value) + 1;
        if (current_cup_it == cups.end()) current_cup_it = cups.begin();

    }

    cout << "final" << endl;

    print_cups(cups);
    cout << endl;

    //Get answer, cups after cup 1, excluding it
    auto it1 = find(cups.begin(), cups.end(), 1);
    
    string solution="";

    for (int i = 0; i < cups.size() - 1; i++) {
        it1++;
        if (it1 == cups.end()) {
            it1 = cups.begin();
        }
        solution += to_string(*it1);
    }

    cout << "Solution: " << solution << endl;


}

void part2(game_cups cups, const uint nmoves) {
    cout << "part2" << endl;

    //Game cups are only the initial
    
    //Setup cups
    cout << "Initial game cups are ";
    uint cup_min = cups[0]; uint cup_max = cups[0];
    for (const auto& c0 : cups) {
        cout << c0 << ",";
        cup_min = min(c0, cup_min);
        cup_max = max(c0, cup_max);
    }
    cout << endl;
    cout << "cup_min " << cup_min << " cup_max " << cup_max << endl;

    //const int nmoves = 100;
    auto remain_cups_count = 1000000 - cups.size();

    for (int i = 0; i < remain_cups_count; i++) {
        cups.push_back(cup_max + 1 + i);
    }

    cup_max = *(cups.end()-1);

    auto current_cup_it = cups.begin() + 0; //index 0

    for (int i = 0; i < nmoves; i++) {

        //picks 3 cups next to current cup
        // cups are removed and spacing is maintained
        if (i % 100==0) {
            cout << "move " << i << endl;
        }

        //print_cups(cups);
        //cout << endl;

        auto current_cup_value = *current_cup_it;

        game_cups picked;
        for (int ipick = 0; ipick < 3; ipick++) {
            auto pick_it = current_cup_it + 1;
            if (pick_it == cups.end()) pick_it = cups.begin();

            auto pick = *pick_it;
            cups.erase(pick_it);
            picked.push_back(pick);

            //Maintain reference
            current_cup_it = find(cups.begin(), cups.end(), current_cup_value);
        }

        //cout << "picked "; print_cups(picked); cout << endl;

        int dest_cup_value = current_cup_value - 1;
        //cout << "dest cup " << dest_cup_value << endl;


        while (find(picked.begin(), picked.end(), dest_cup_value) != picked.end() ||
            find(cups.begin(), cups.end(), dest_cup_value) == cups.end()) {
            dest_cup_value -= 1;
            if (dest_cup_value < (int)cup_min) {
                dest_cup_value = cup_max;
            }
        }

        //destination cup location, from iterators
        auto dest_loc_it = find(cups.begin(), cups.end(), dest_cup_value) + 1;

        cups.insert(dest_loc_it, picked.begin(), picked.end());

        // Find iterator of current cup, and set the next
        current_cup_it = find(cups.begin(), cups.end(), current_cup_value) + 1;
        if (current_cup_it == cups.end()) current_cup_it = cups.begin();

    }

    cout << "final" << endl;

    print_cups(cups);
    cout << endl;

    //Get answer, cups after cup 1, excluding it
    auto it1 = find(cups.begin(), cups.end(), 1);

    string solution = "";

    for (int i = 0; i < cups.size() - 1; i++) {
        it1++;
        if (it1 == cups.end()) {
            it1 = cups.begin();
        }
        solution += to_string(*it1);
    }

    cout << "Solution: " << solution << endl;

    //TOO SLOW

}


// Previous solution was too slow.
// Rather than inserting and erasing elements I will try the approach of nodes and next/previous iterators
// Also have a map from value to the object

struct cCup {
    cCup* next_cup;
    //cCup* previous_cup;
    uint value = 0;

};


void part2_2(game_cups cups, const uint nmoves, uint total_cups) {
    cout << "part2_2" << endl;
    unordered_map<uint, cCup*> cup_dict;
    deque<cCup*> ccups_initial;

    
    //create ccups and store in deque
    uint cup_min = cups[0]; uint cup_max = cups[0];
    for (const auto& c0 : cups) {
        auto ccup = new cCup();

        ccup->value = c0;
        ccups_initial.push_back(ccup);
        cup_dict[c0] = ccup;
        
        cup_min = min(c0, cup_min);
        cup_max = max(c0, cup_max);
    }

    //const int nmoves = 100;
    auto remain_cups_count = total_cups - cups.size();

    auto old_max = cup_max;

    for (uint i = 0; i < remain_cups_count; i++) {
        auto ccup = new cCup();

        auto v0 = old_max + 1 + i;
        ccup->value = v0;

        cup_dict[v0] = ccup;
        ccups_initial.push_back(ccup);

        cup_max = max(v0, cup_max);
    }

    //cup_max = *(cups.end() - 1); //last value is the maximum
    cout << "game_cups count " << ccups_initial.size() << ", max cup value " << cup_max << endl;

    // associate all of them with next and previous
    //auto it_previous = ccups_initial.end();
    auto it_current = ccups_initial.begin();
    
    for (int i=0; i< ccups_initial.size(); i++){
        auto it_next = it_current + 1;
        if (it_next == ccups_initial.end()) {
            it_next = ccups_initial.begin();
        }

        //(*it_current).previous_cup = &(*it_previous);
        (*it_current)->next_cup = *it_next;

        //next iteration (cyclic)
        it_current++;

    }


    //By this point the nodes and links should all be set

    auto cCup_p0 = ccups_initial[0]; //initial
    //decltype(cCup_p0) cCup_p1;

    for (int i = 0; i < nmoves; i++) {
        
        if ((i % 100000) == 0) {
            cout << "Move " << (i + 1) << endl;
        }
        //cout << "Current cup " << cCup_p0->value << endl;

        //get the 3 cups next to initial and remove them by changing next and previous
        decltype(cCup_p0) cCup_p1 = cCup_p0->next_cup;
        decltype(cCup_p0) cCup_p3 = cCup_p1->next_cup->next_cup; //last of picked cups

        decltype(cCup_p0) cCup_p4 = cCup_p3->next_cup;

        //connect current to 4
        cCup_p0->next_cup = cCup_p4;

        int v0 = (cCup_p0->value) - 1;
        if (v0 < cup_min) {
            v0 = cup_max;
        }

        //Collect values of picked
        unordered_set<uint> picked_values = { cCup_p1->value, cCup_p1->next_cup->value , cCup_p3->value };

        while (picked_values.find(v0) != picked_values.end()) {
            v0--;

            if (v0 < cup_min) {
                v0 = cup_max;
            }
        }

        //cout << "Destination " << v0 <<endl;

        //Assume that v0 has a value of a cup that was not picked, use dict to help

        decltype(cCup_p0) cCup_dest = cup_dict[(uint)v0];
        decltype(cCup_dest) cCup_dest_next = cCup_dest->next_cup;

        //Insert the picked cups
        cCup_dest->next_cup = cCup_p1;
        cCup_p3->next_cup = cCup_dest_next;

        //Select the new current cup to be the next of previous current
        cCup_p0 = cCup_p0->next_cup;

    }

    auto cup0 = cup_dict[1];

    //cout << "Completed moves. The final cup order from cup 1 is: " << endl;
    //for (int i = 0; i < ccups_initial.size(); i++) {
    //    cout << cup0->value << ",";
    //    cup0 = cup0->next_cup;
    //}

    //Calculate solution

    auto v0 = cup0->next_cup->value;
    auto v1 = cup0->next_cup->next_cup->value;

    cout << "values are " << v0 << "," << v1 <<endl;

    uintll solution = (uintll)v0 * (uintll)v1;
    cout << "Solution " << solution << endl;


}

int main() {
    //Read data from file to list of strings

    //string input_file = "input.txt";
    //if (TEST) {
    //    input_file = test_file;
    //}
    //auto slines = get_lines_from_file(input_file.c_str());

    //parse_input_lines(slines);

    //game_cups cups = { 3,2,4,1,5 };
    //game_cups cups = { 3,8,9,1,2,5,4,6,7 }; //test, part2: 934001, 159792, 149245887792
    game_cups cups = { 4,6,9,2,1,7,5,3,8 }; //input



    //part1(cups,10); //ok
    //part1(cups, 100);


    //part2_2(cups, 10, cups.size());
    //part2_2(cups, 100, cups.size());
    part2_2(cups, 10000000, 1000000);

    return 0;
}