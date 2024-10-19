// https://adventofcode.com/2020/day/15

#include "../aoc_lmap_utils.h"

void part1(vector<int> input) {

	//Implement the spoken numbers as an hashmap
	// with key being the number, and value being time index last spoken

	//Deal with initial input

	unordered_map<int, int> spokenlog;

	for (int i = 0; i < input.size(); i++) {
		auto v = input[i];

		//Add element
		spokenlog[v] = i;
	}

	int nextval = 0;
	//int spoken_at = -1; //-1 for never spoken
	//int last_val = 0;

	for (int j = input.size(); j < 2019; j++) {
		//Check if nextval was ever spoken

		auto nextval1 = nextval; //ensures same type
		auto f0 = spokenlog.find(nextval);
		if (f0 == spokenlog.end()) {
			//never spoken
			nextval1 = 0;
		}
		else {
			//nextval will have difference
			nextval1 = j - f0->second; //get value (index)
		}
		spokenlog[nextval] = j;

		cout << "j:" << j << " , nextval:" << nextval << " , nextval1:" << nextval1 << endl;
		nextval = nextval1;
		
	}
	cout << "Answer is " << nextval<<endl;
}

void part2(vector<int> input) {
	//same as part1 but with 30000000 iterations

	unordered_map<uintl, uintl> spokenlog;

	for (uintl i = 0; i < input.size(); i++) {
		auto v = input[i];

		//Add element
		spokenlog[v] = i;
	}

	uintl nextval = 0;
	//int spoken_at = -1; //-1 for never spoken
	//int last_val = 0;

	for (uintl j = input.size(); j < 30000000-1; j++) {
		//Check if nextval was ever spoken

		auto nextval1 = nextval; //ensures same type
		auto f0 = spokenlog.find(nextval);
		if (f0 == spokenlog.end()) {
			//never spoken
			nextval1 = 0;
		}
		else {
			//nextval will have difference
			nextval1 = j - f0->second; //get value (index)
		}
		spokenlog[nextval] = j;

		//cout << "j:" << j << " , nextval:" << nextval << " , nextval1:" << nextval1 << endl;
		nextval = nextval1;

	}
	cout << "Answer is " << nextval << endl;
}

int main() {
	// inputs
	vector<int> input_test = { 0,3,6 };
	vector<int> input = { 1,12,0,20,8,16 };

	//part1(input_test);
	//part1(input); //ok

	//part2(input_test); //expected 175594. OK
	part2(input);
}