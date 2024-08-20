#include <cstdio>
#include <fstream> //read write files
#include <string>
#include <vector>
#include <map>
#include <sstream>

#define SUM_TARGET 2020

using namespace std;

int main(){

    //Read data from file to list of strings

    //ifstream my_file("input_test.txt"); // Open the file
    ifstream my_file("input.txt"); // Open the file
    vector<int> values;
    string sline;

    if (my_file.is_open()){
        while (getline(my_file, sline)){
            //slines.push_back(line)
            int ivalue;
            istringstream iss(sline);
            while (iss >> ivalue) {
                values.push_back(ivalue);
            }
        }
        my_file.close();

    }else{
        perror("Unable to open file");
        return 1;
    }

    // Print the numbers
    for (auto ivalues : values) {
        printf("%d\n", ivalues);
    }

    //Find the combination that sum gives required value

    for (int i=0; i<values.size()-1; i++){
        for (int j=i+1;j<values.size(); j++){
            int iv = values[i];
            int jv = values[j];
            if (iv+jv == SUM_TARGET){
                printf("Combination found %d: %d, %d:%d \n", i, iv, j, jv);
                long mult = iv*jv;
                printf("Multiplying values together gives %d \n", mult);
                break;
            }
        }
    }
    return 0;
}

