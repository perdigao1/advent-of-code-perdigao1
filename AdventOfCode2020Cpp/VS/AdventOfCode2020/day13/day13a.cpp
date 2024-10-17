// https://adventofcode.com/2020/day/13

#include "../aoc_lmap_utils.h"

using namespace std;
#include <cmath>
#include <cassert>

#include <numeric> // for std::gcd, make sure C++ version >=17


void part1(vector<string> slines) {
    //get first line being the earliest time
    uint earliest_time = stoi(slines[0]);

    //to get valid buses, split second string
    string s_buses = slines[1];

    //can use C style strtok, but will use the regex

    auto matches0 = regex_get_matches_and_their_groups(s_buses, "[0-9]+");

    vector<uint> buses;
    cout << "buses" << endl;
    for (auto m : matches0) {
        cout << m.match << ",";
        buses.push_back(stoi(m.match));
    }

    //Get the first bus from timestamp
    uint bus_time_first = 0;
    uint earliest_bus_id = 0;

    for (uint i = 0; i < buses.size(); i++) {
        auto bus_number = buses[i];

        uint bus_earliest_time = (floor(float(earliest_time) / float(bus_number)) + 1) * bus_number;

        cout << "bus number " << bus_number << " earliest time " << bus_earliest_time << endl;

        if (i == 0) {
            bus_time_first = bus_earliest_time;
            earliest_bus_id = bus_number;
        }
        else if (bus_earliest_time < bus_time_first) {
            bus_time_first = bus_earliest_time;
            earliest_bus_id = bus_number;
        }
    }

    uint wait = bus_time_first - earliest_time;

    cout << "earliest bus id " << earliest_bus_id << " , wait " << wait << endl;

    cout << "answer " << earliest_bus_id * wait;
}



// Code below for the Chinese remainder algorithm is from ChatGPT
// and is inspired by the Sympy crt() function

// Extended Euclidean Algorithm to find gcd and the coefficients of Bézout's identity
tuple<uintll, uintll, uintll> extendedGcd(uintll a, uintll b) {
    if (b == 0)
        return { a, 1, 0 }; // gcd(a, 0) = a, x = 1, y = 0
    auto [g, x1, y1] = extendedGcd(b, a % b);
    return { g, y1, x1 - (a / b) * y1 };
}

// Modular inverse function using the Extended Euclidean Algorithm
uintll modInverse(uintll a, uintll m) {
    auto [g, x, y] = extendedGcd(a, m);
    if (g != 1) {
        throw invalid_argument("Modular inverse does not exist");
    }
    return (x % m + m) % m; // Ensure the inverse is positive
}

// Function to solve a system of congruences using the Chinese Remainder Theorem (works for non-coprime moduli)
tuple<uintll, uintll> crt(const vector<uintll>& num, const vector<uintll>& rem) {
    // This code is not working correctly
    uintll x = rem[0]; // Initialize with the first remainder
    uintll mod = num[0]; // Initialize with the first modulus

    for (size_t i = 1; i < num.size(); ++i) {
        uintll n = num[i];
        uintll r = rem[i];

        auto [g, m1_inv, m2_inv] = extendedGcd(mod, n); // gcd(mod, n)

        if ((r - x) % g != 0) {
            // No solution exists if the difference is not divisible by gcd
            throw invalid_argument("No solution exists for the given system of congruences");
        }

        // Update the solution using the formula for non-coprime moduli
        x += mod * ((r - x) / g * m1_inv % (n / g)); // Combine the solutions
        mod = mod * (n / g); // Least common multiple of mod and n

        x = (x % mod + mod) % mod; // Ensure x is positive
    }

    return { x, mod }; // x is the solution, mod is the least common multiple of all moduli
}


// This code below is simple and works
uintll crt_by_sieving(const vector<uintll>& mods, const vector<uintll>& rem) {
    // Use method describe here
    // https://en.wikipedia.org/wiki/Chinese_remainder_theorem#Search_by_sieving 

    // First thing is to fix remainders so they are not higher than respective mods

    auto rems_fixed = rem; //copy?

    for (int i = 0; i < mods.size(); i++) {
        rems_fixed[i] = rem[i] % mods[i];
    }

    // Do the algorithm by sieving.
    uintll nmult=0;
    uintll x=0;

    for (int i = 0; i < mods.size(); i++) {
        // get the mod we want find congruence
        auto m0 = mods[i];
        auto r0 = rems_fixed[i];
        
        cout << "i:" << i << " , m0:" << m0 << " , r0:" << r0 <<
            " , nmult:"<< nmult << " , x:"<< x << endl;

        if (i == 0) {
            //initialise nmult and x
            nmult = m0;
            x = r0; // x(=r0) mod m0 = r0
        }
        else {
            // check nmult*k until correct mod is found
            uintll k = 0;
            while (true) {
                auto check0 = k * nmult + x;
                auto check_mod = check0 % m0;
                cout << "k:" << k << " , check0:" << check0 << " , check_mod:" << check_mod<< endl;

                if (check_mod != r0) {
                    k += 1;
                }
                else {
                    cout << "found congr with " << check0 << endl;
                    nmult *= m0;
                    x = check0;
                    break;
                }
                //if (k > nmult) {
                //    cout << "k > nmult .  something wrong. continue?" << endl;
                //    char c;
                //    cin >> c;
                //    if (c != 'y') {
                //        exit(0);
                //    }
                //}
            }
        }
    }
    cout << "Solution found by sieving is x=" << x << endl;
    return x;
}

void part2(vector<string> slines) {
    //to get valid buses, split second string
    string s_buses = slines[1];

    //can use C style strtok, but will use the regex

    auto matches0 = regex_get_matches_and_their_groups(s_buses, "[0-9x]+");

    map<uint, uint> t_busid;

    uint k = matches0.size();

    for (uint i = 0; i < matches0.size(); i++) {
        cout << "i:" << i << " match: " << matches0[i].match << endl;
        auto m = matches0[i].match;

        if (m != "x") {
            auto v = stoi(m);
            t_busid[i] = v;
        }
    }

    // Create two seperate arrays, num and rem
    vector<uintll> nums;
    vector<uintll> rems;

    for (auto m0 : t_busid) {
        nums.push_back(m0.second);
        rems.push_back(k - 1 - m0.first);
    }

    cout << "nums: ";
    for (auto n0 : nums) {
        cout << n0 << ", ";
    }
    cout << endl;

    cout << "rems: ";
    for (auto r0 : rems) {
        cout << r0 << ", ";
    }
    cout << endl;

    cout << "Using now the Chinese remainder theorem" << endl;

    // auto [res, mod0] = crt(nums, rems);
    
    uintll mod0 = 0;
    auto res = crt_by_sieving(nums, rems);

    auto finalres = res - k + 1;

    cout << "Result is " << finalres << " mod0: " << mod0 << endl;

    // Check correctness
    cout << "Check correctness" << endl;
    for (int i = 0; i < nums.size(); i++) {
        auto n0 = nums[i]; //modulii
        auto r0 = rems[i]; //remainders

        auto calc_mod = res % n0;
        auto rem_mod = r0 % n0;

        cout << "mod, rem: " << n0 << ", " << r0 << "   VS calc mod,rem: " << calc_mod << " , " << rem_mod ;
        if (calc_mod == rem_mod) {
            cout << " CORRECT" << endl;
        }
        else {
            cout << " WRONG" << endl;
        }
    }
}



int main() {
    //Read data from file to list of strings

    //auto slines = get_lines_from_file("input_test.txt"); // 1068781
    //auto slines = get_lines_from_file("input_test1.txt"); // 3417
    //auto slines = get_lines_from_file("input_test2.txt");

    auto slines = get_lines_from_file("input.txt"); // 1535242681 not correct,
    // 263680799288946 not correct
    // 1063858958135574 not correct
    // 1037773467751708 not correct
    // 1063858958135574
    // 
    // sympy crt
    //800177252346292 ? no, forgot to subtract
    // 1800183506587661 ?
    //800177252346225 Correct

    //part1(slines);
    part2(slines);

    //cout << "max unsigned long long is " << LLONG_MAX;

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
