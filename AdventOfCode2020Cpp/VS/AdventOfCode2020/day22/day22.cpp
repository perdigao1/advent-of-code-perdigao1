// https://adventofcode.com/2020/day/21

#include "../aoc_lmap_utils.h"

//part2 test 291, run 33805


bool TEST = false;
string test_file = "input_test.txt"; //9 tiles

using deck = deque<uint>;

deck player1;
deck player2;
decltype(player1) winning_dec;

void parse_input_lines(strings_t slines) {
    cout << "parse_input_lines()" << endl;
 
    uint parse_player = 0; // { NONE=0, PLAYER1=1, PLAYER2=2 };

    for (const auto& s : slines) {
        if (s.size() > 0) {
            if (s.find("Player ") != s.npos) {
                string s0 = s.substr(7, 1);
                parse_player = stoi(s0);
            }
            else {
                //Assume is number
                uint card_value = stoi(s);
                if (parse_player == 1) {
                    player1.push_back(card_value);
                }
                else if (parse_player == 2) {
                    player2.push_back(card_value);
                }
            }
        }
    }

    cout << "Input parsed. This is the cards for each player" << endl;
    cout << "Player 1" << endl;
    for (auto p0 : player1) {
        cout << p0 << ",";
    }
    cout << endl;

    cout << "Player 2" << endl;
    for (auto p0 : player2) {
        cout << p0 << ",";
    }
    cout << endl;
}

void part1() {
    cout << "part1" << endl;

    // Do the rounds
    bool no_winer = true;
    uint round_count = 0;

    while (no_winer) {
        round_count++;
        auto top1 = player1.front();
        player1.pop_front();
        
        auto top2 = player2.front();
        player2.pop_front();

        cout << top1 << " vs " << top2;

        if (top1 > top2) {
            //to the back in the deck in order
            cout << ". player 1 wins round" << endl;
            player1.push_back(top1);
            player1.push_back(top2);
        }
        else {
            cout << ". player 2 wins round" << endl;
            player2.push_back(top2);
            player2.push_back(top1);
        }

        cout << "Round " << round_count << " completed" << endl;



        if (player1.size() == 0) {
            //player 2 wins
            cout << "player 2 wins the game" << endl;
            winning_dec = player2;
            no_winer = false;
        }
        else if(player2.size()==0){
            cout << "player 1 wins the game" << endl;
            winning_dec = player1;
            no_winer = false;
        }
    }

    uint calc_sum = 0;
    uint calc_count = 1;
    //Calculate solution
    for (auto it = winning_dec.crbegin(); it != winning_dec.crend(); it++) {
        auto value = *it;
        calc_sum += calc_count * value;
        calc_count++;

    }
    cout << "Score " << calc_sum << endl;


}

//Need to have a way to store the rounds and check equality
deque<pair<deck, deck>> old_rounds;


void print_deck(deck d) {
    for (const auto& d0 : d) {
        cout << d0 << ",";
    }
}


//unordered_map<pair<deck, deck>, uint> p2_round_wins_mem; //needs costum hash function


uint recursive_winner_is(deck  player1, deck player2, uint level) {
    //TODO: TAKES TOO LONG, MAYBE SOME MEMOISATION WOULD HELP


    level++;
    //cout <<  endl;

    uint round = 0;

    // Do the rounds
    bool no_winer = true;
    uint round_count = 0;
    deque<pair<deck, deck>> old_rounds;

    while (player1.size() > 0 && player2.size() > 0) {
        round_count++;
        uint round_winner = 0;

        //cout << "Game level " << level << ". Round " << round_count << endl;

        //cout << "P1 deck: "; print_deck(player1); cout << endl;
        //cout << "P2 deck: "; print_deck(player2); cout << endl;

        //Check against old_rounds within this game

        for (const auto& old_round : old_rounds) {
            auto old_p1 = old_round.first;
            auto old_p2 = old_round.second;

            if (old_p1 == player1 && old_p2 == player2) { //the == operator in deque compares all elements and sizes
                //winner is player 1
                //cout << "Round equal to an old round. Player 1 is the winner." << endl;
                return 1;
            }
        }
        //New round
        old_rounds.push_back({ player1, player2 });

        auto top1 = player1.front();
        player1.pop_front();

        auto top2 = player2.front();
        player2.pop_front();

        //cout << top1 << " vs " << top2<< endl;

        //Check numbers to see if it is required to go recursive.
        if (player1.size() >= top1 && player2.size() >= top2) {
            ///go recursive
            //cout << "Playing subgame to determine the winner" << endl;

            decltype(player1) pl1_rec;
            pl1_rec.insert(pl1_rec.end(), player1.begin(), player1.begin() + top1);

            decltype(player2) pl2_rec;
            pl2_rec.insert(pl2_rec.end(), player2.begin(), player2.begin() + top2);

            round_winner = recursive_winner_is(pl1_rec, pl2_rec, level);

        }
        else {

            if (top1 > top2) {
                //to the back in the deck in order
                round_winner = 1;
            }
            else {
                round_winner = 2;
            }

        }

        if (round_winner == 1) {
            //cout << "player 1 wins round" << endl;
            player1.push_back(top1);
            player1.push_back(top2);
        }
        else if (round_winner == 2) {
            //cout << "player 2 wins round" << endl;
            player2.push_back(top2);
            player2.push_back(top1);
        }
    }

    uint winner = 0;
    if (player1.size() == 0) {
        //player 2 wins
        //cout << "player 2 wins the game" << endl;
        winning_dec = player2;
        //no_winer = false;
        winner = 2;
    }
    else if (player2.size() == 0) {
        //cout << "player 1 wins the game" << endl;
        winning_dec = player1;
        //no_winer = false;
        winner = 1;
    }

    return winner;
}




//Using deepseek
// Hash function for deck
struct deck_hash {
    std::size_t operator()(const deck& d) const {
        std::size_t seed = d.size();
        for (const auto& elem : d) {
            seed ^= std::hash<uint>{}(elem)+0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

// Hash function for pair<deck, deck>
struct pair_deck_hash {
    std::size_t operator()(const pair<deck, deck>& p) const {
        deck_hash dh;
        return dh(p.first) ^ (dh(p.second) << 1);
    }
};

// Equality for pair<deck, deck>
struct pair_deck_equal {
    bool operator()(const pair<deck, deck>& lhs,
        const pair<deck, deck>& rhs) const {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};


std::unordered_map<
    pair<deck, deck>,
    uint,
    pair_deck_hash,
    pair_deck_equal
> p2_round_wins_mem;

uint recursive_winner_is_with_mem(deck  player1, deck player2, uint level) {
    //TODO: TAKES TOO LONG, MAYBE SOME MEMOISATION WOULD HELP
    pair<deck,deck> pair12({ player1, player2 });

    //check copies for memory
    if (p2_round_wins_mem.find({ pair12 }) != p2_round_wins_mem.end()) {
        auto win = p2_round_wins_mem[pair12];
        //cout << "Historic round already processed. Winner is " << win<< endl;
        return win;

    }else {
        //creates copies
        deck player1_0(player1);
        deck player2_0(player2);
    
        level++;
        //cout <<  endl;

        uint round = 0;

        // Do the rounds
        bool no_winer = true;
        uint round_count = 0;
        deque<pair<deck, deck>> old_rounds;

        while (player1.size() > 0 && player2.size() > 0) {
            round_count++;
            uint round_winner = 0;

            //cout << "Game level " << level << ". Round " << round_count << endl;

            //cout << "P1 deck: "; print_deck(player1); cout << endl;
            //cout << "P2 deck: "; print_deck(player2); cout << endl;

            //Check against old_rounds within this game

            for (const auto& old_round : old_rounds) {
                auto old_p1 = old_round.first;
                auto old_p2 = old_round.second;

                if (old_p1 == player1 && old_p2 == player2) { //the == operator in deque compares all elements and sizes
                    //winner is player 1
                    //cout << "Round equal to an old round. Player 1 is the winner." << endl;
                    return 1;
                }
            }
            //New round
            old_rounds.push_back({ player1, player2 });

            auto top1 = player1.front();
            player1.pop_front();

            auto top2 = player2.front();
            player2.pop_front();

            //cout << top1 << " vs " << top2<< endl;

            //Check numbers to see if it is required to go recursive.
            if (player1.size() >= top1 && player2.size() >= top2) {
                ///go recursive
                //cout << "Playing subgame to determine the winner" << endl;

                decltype(player1) pl1_rec;
                pl1_rec.insert(pl1_rec.end(), player1.begin(), player1.begin() + top1);

                decltype(player2) pl2_rec;
                pl2_rec.insert(pl2_rec.end(), player2.begin(), player2.begin() + top2);

                round_winner = recursive_winner_is_with_mem(pl1_rec, pl2_rec, level);

            }
            else {

                if (top1 > top2) {
                    //to the back in the deck in order
                    round_winner = 1;
                }
                else {
                    round_winner = 2;
                }

            }

            if (round_winner == 1) {
                //cout << "player 1 wins round" << endl;
                player1.push_back(top1);
                player1.push_back(top2);
            }
            else if (round_winner == 2) {
                //cout << "player 2 wins round" << endl;
                player2.push_back(top2);
                player2.push_back(top1);
            }
        }

        uint winner = 0;
        if (player1.size() == 0) {
            //player 2 wins
            //cout << "player 2 wins the game" << endl;
            winning_dec = player2;
            //no_winer = false;
            winner = 2;
        }
        else if (player2.size() == 0) {
            //cout << "player 1 wins the game" << endl;
            winning_dec = player1;
            //no_winer = false;
            winner = 1;
        }

        //use memory
        pair<deck, deck> pair0({ player1_0, player2_0 });
        p2_round_wins_mem[pair0] = winner;
        
        return winner;
    }
}

void part2() {
    cout << "part2" << endl;

    //cout << "Using function recursive_winner_is(). Running..." << endl;
    //auto res = recursive_winner_is(player1, player2, 0);

    cout << "Using function recursive_winner_is_with_mem(). Running..." << endl;
    auto res = recursive_winner_is_with_mem(player1, player2, 0);

    cout << "part2 winner is " << res<<endl;

    uint calc_sum = 0;
    uint calc_count = 1;
    //Calculate solution
    for (auto it = winning_dec.crbegin(); it != winning_dec.crend(); it++) {
        auto value = *it;
        calc_sum += calc_count * value;
        calc_count++;

    }
    cout << "Score " << calc_sum << endl;
}

int main() {
    //Read data from file to list of strings

    string input_file = "input.txt";
    if (TEST) {
        input_file = test_file;
    }
    auto slines = get_lines_from_file(input_file.c_str());

    parse_input_lines(slines);

    //part1();

    part2();


    return 0;
}