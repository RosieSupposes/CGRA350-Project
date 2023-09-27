#include "Rule.hpp"

using namespace std;

Rule::Rule(string input, string output){
    this->input = input;
    this->output = output;
}

Rule::~Rule() {
}

string Rule::applyRule(string command){
    string newCommand;
    int j = 0;
    
    for (int i = 0; i < (int)command.size(); i++) {
        if (command.at(i) == input.at(j) || input.at(j) == '_') {// '_' is the special character that means any character
            j++;
        }
        else {
            for (; j >= 0; j--) {
                newCommand.push_back(command.at(i - j));
            }
            j = 0;
        }
        if (j == (int)input.size()) {
            for (int k = 0; k < (int)output.size(); k++) {
                newCommand.push_back(output.at(k));
            }
            j = 0;
        }
    }
    return newCommand;
}
