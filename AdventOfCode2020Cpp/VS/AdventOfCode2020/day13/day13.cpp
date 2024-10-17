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
        
        uint bus_earliest_time = (floor(float(earliest_time) / float(bus_number)) + 1)* bus_number;

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

/*
// Use chinese remainder theorem, and reference end value rather than start
//
// Function to find the modular inverse of a with respect to m using the Extended Euclidean Algorithm
// https://www.geeksforgeeks.org/implementation-of-chinese-remainder-theorem-inverse-modulo-based-implementation/

uintll modInverse(uintll a, uintll m) {
    uintll m0 = m, t, q;
    uintll x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0)
        x1 += m0;

    return x1;
}

// Function to find the smallest x such that:
// x % num[i] = rem[i] for all i
uintll findMinX(vector<uint> num, vector<uint> rem) {
    assert (num.size() == rem.size());

    auto k = num.size();

    uintll prod = 1;
    for (int i = 0; i < k; i++)
        prod *= num[i];

    cout << "prod: " << prod << endl;

    uintll result = 0;
    for (int i = 0; i < k; i++) {
        uintll pp = prod / num[i];
        result += rem[i] * modInverse(pp, num[i]) * pp;
    }

    return result % prod;
}
*/


// From Github Co-pilot
// Version that checks numbers are coprime and fixes


// Function to find modular inverse of a under modulo m
uintll modInverse(uintll a, uintll m) {
    uintll m0 = m, t, q;
    uintll x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

// Function to check and fix coprimeness
void ensureCoprime(vector<uint>& num) {
    for (size_t i = 0; i < num.size(); ++i) {
        for (size_t j = i + 1; j < num.size(); ++j) {
            while (gcd((uintll)num[i], (uintll)num[j]) != 1) {
                num[j]++;
            }
        }
    }
}

// Function to find the smallest x such that:
// x % num[i] = rem[i] for all i
uintll findMinX(vector<uint>& num, vector<uint>& rem) {
    ensureCoprime(num);

    uintll prod = 1;
    for (uintll n : num) prod *= n;
    
    uintll result = 0;
    for (size_t i = 0; i < num.size(); i++) {
        uintll n = num[i];
        uintll pp = prod / n;
        uintll r0 = rem[i];
        result += r0 * modInverse(pp, n) * pp;
    }
    return result % prod;
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
    vector<uint> nums;
    vector<uint> rems;

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

    auto res = findMinX(nums, rems);

    auto finalres = res - k + 1;

    cout << "Result is " << finalres << endl;

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
    //800177252346225 ? Correct

    //part1(slines);
    part2(slines);

    cout << "max unsigned long long is " << LLONG_MAX;

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
