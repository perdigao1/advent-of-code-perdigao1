// https://adventofcode.com/2020/day/14

#include "../aoc_lmap_utils.h"

using namespace std;
#include <cmath>
#include <cassert>

#include <numeric> // for std::gcd, make sure C++ version >=17


//Implement current mask as a singleton class
// with help of github copilot
class Mask {
private:
    static Mask* instance;
    Mask() {}  // Private constructor
    uintll mask0;
    uintll mask1;
    // mask1, mask 0 combinations
    // 1,0 = 0 ; 1,1 = 1 ; 0,1=X ; 0,0=outside
    // result = m1&m0 or ~m1&v

public:
    Mask(const Mask&) = delete;  // Delete copy constructor
    Mask& operator=(const Mask&) = delete;  // Delete assignment operator

    static Mask* getInstance() {
        if (instance == nullptr) {
            instance = new Mask();
        }
        return instance;
    }

    //Functions
    void setMask(const string& mask_str) {
        //read mask with X zeros and 1's and use it later to mask output
        //set mask 0
        string mask0_str(36, '0');
        string mask1_str(36, '0');

        for (int i = 0; i < 36; i++) {
            auto c = mask_str[i];
            switch (c){
            case 'X':
                mask0_str[i] = '1';
                mask1_str[i] = '0';
                break;
            case '1':
                mask0_str[i] = '1';
                mask1_str[i] = '1';
                break;
            case '0':
                mask0_str[i] = '0';
                mask1_str[i] = '1';
                break;
            default:
                cerr << "Can't interpret character " << c << " at " << i << endl;
            } 
        }
        cout << "New mask" << endl;
        cout << "mask0: " << mask0_str<<endl;
        cout << "mask1: " << mask1_str<<endl;

        mask0 = std::stoll(mask0_str, nullptr, 2);
        mask1 = std::stoll(mask1_str, nullptr, 2);

    }

    uintll applyMaskTo(uintll value) {
        return (mask1 & mask0) | (~mask1 & value);
    }

};

// Initialize pointer to zero so that it can be initialized in first call to getInstance
Mask* Mask::instance= nullptr;


void part1(vector<string> slines) {
    
    Mask* the_mask = Mask::getInstance();

    // parse file line-by-line (input is already like that)
    // for each line read either mask or mem
    //parse and process

    //implement the memory as a hasmap
    unordered_map <int, uintll> memory;

    for (auto s : slines) {
        if (s.size() < 0) {
            continue; //skip line
        }
        auto s0 = s.substr(0, 3);


        if (s0 == "mas") {
            //mask
            string pat0 = "[X10]{36}";

            auto reg0 = regex_get_matches_and_their_groups(s, pat0);

            if (reg0.size() != 1) {
                cerr << "Error getting binary mask with line:" << s0 <<endl;
                continue;
            }

            auto match0 = reg0[0].match; //assume is a nice string

            // set the current mask
            the_mask->setMask(match0); //error
            //cout << "New mask set" << endl;

        }

        else if (s0 == "mem") {
            string pat0 = "mem\\[([0-9]+)\\]\\s*=\\s*([0-9]+)";
            auto reg0 = regex_get_matches_and_their_groups(s, pat0);

            //expected single match with 2 groups
            if (reg0.size() != 1) {
                if (reg0[0].groups.size() != 2) {
                    cerr << "Error getting binary mask with line:" << s0 << endl;
                    continue;
                }
            }
            auto mem_pos = stoi(reg0[0].groups[0]);
            uintll new_value = stoll(reg0[0].groups[1]);

            cout << "mem position: " << mem_pos << " , new_value: " << new_value;

            //mask value
            auto masked_value = the_mask->applyMaskTo(new_value);
            cout << ", masked : " << masked_value << endl;

            memory[mem_pos] = masked_value;

        }

    }

    //Get the sum
    uintll sum = 0;

    for (const auto& m : memory) {
        sum += m.second;
    }
    
    cout << "sum is : " << sum << endl;

}

void part2(vector<string> slines) {
    //Different way to process
    //bitmask acts on the memory address, not on the value

    //X is floating, can take any value
}



int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test.txt");

    auto slines = get_lines_from_file("input.txt"); // 1535242681 not correct,

    part1(slines);

    // part2(slines);

    return 0;
}


/*
*
* I am getting wrong results when using the chinese remainder theorem
*


nums: 13, 41, 641, 19, 17, 29, 661, 37, 23,
rems: 67, 64, 54, 42, 37, 25, 23, 17, 0,

results value 1063858958135574
(note that the result here is the crt() - 67 )

but sympy crt function gives
800177252346225
which is the correct value

*/
