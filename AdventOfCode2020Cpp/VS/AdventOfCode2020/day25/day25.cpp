// https://adventofcode.com/2020/day/25

#include "../aoc_lmap_utils.h"



// Rather than creating a grid, I will store changed tiles to black in a set.

bool TEST = false;

const uintll div0 = 20201227;
const uintll sn = 7; // subject number


unordered_map<uintll, uintll> key_to_loopnumber;

void generete_map_key_to_loopnumber() {
    //Check loop sizes
    auto v0 = 1;
    for (int i = 0; i < div0; i++) {

        v0 = (v0 * sn) % div0;
        //cout << "i: " << i + 1 << " , v0: " << v0 << endl;

        key_to_loopnumber[v0] = i+1;
    }
}

uintll get_key(uintll subj_n, uintll loop_count) {
    auto v0 = 1;
    for (int i = 0; i < loop_count; i++) {

        v0 = (v0 * subj_n) % div0;
        //cout << "i: " << i + 1 << " , v0: " << v0 << endl;
    }
    return v0;
}

void part1(pair <uintll, uintll> inputs) {
    cout << "part1" << endl;

    auto card_pk = inputs.first;
    auto door_pk = inputs.second;

    generete_map_key_to_loopnumber();

    auto card_loop = key_to_loopnumber[card_pk];
    auto door_loop = key_to_loopnumber[door_pk];

    cout << "card_loop: " << card_loop << " , " << "door_loop: " << door_loop << endl;

    // calculate key
    auto encr_key0 = get_key(card_pk, door_loop);
    auto encr_key1 = get_key(door_pk, card_loop);

    cout << "encr_key0: " << encr_key0 << " , encr_key1: " << encr_key1 << endl;


    return;
}

void part2(int nrounds) {
    cout << "part2" << endl;

 }

int main() {
    

    pair <uintll, uintll> inputs;
    if (TEST) {
        inputs = { 5764801, 17807724 }; // card_loop: 7, door_loop : 8
    }
    else {
        inputs = { 15113849, 4206373 }; // card_loop: 11796245, door_loop : 1245397
    }

    part1(inputs);


    //part 2: test 2208, 
    //part2(100); //use object black_tiles (initial configuration)


    return 0;
}