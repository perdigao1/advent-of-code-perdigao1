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
    // for (auto ivalues : values) {
    //     printf("%d\n", ivalues);
    // }

    //Find the combination that sum gives required value
    bool b_combination_found=false;

    for (int i=0; i<values.size()-2; i++){
        for (int j=i+1;j<values.size()-1; j++){
            for (int k=j+1; k<values.size(); k++){
                int iv = values[i];
                int jv = values[j];
                int kv = values[k];
                if (iv+jv+kv == SUM_TARGET){
                    printf("Combination found %d: %d, %d:%d , %d:%d\n", i,iv, j,jv, k,kv);
                    long mult = iv*jv*kv;
                    printf("Multiplying values together gives %d \n", mult);
                    b_combination_found=true;
                }
                if (b_combination_found){
                    break;
                }
            }
            if (b_combination_found){
                break;
            }
        }
        if (b_combination_found){
            break;
        }
    }
    return 0;
}

