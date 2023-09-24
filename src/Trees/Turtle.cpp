#include "Turtle.hpp"

void Turtle::loadRules(vector<string> rules){
    for (int i = 0; i < rules.size(); i++) {
        std::string input;
        std::string output;
        bool foundColon = false;
        for (char c : rules.at(i)) {
            if (foundColon) {
                output.push_back(c);
            }
            else {
                if (c == ':') {
                    foundColon = true;
                }
                else {
                    input.push_back(c);
                }
            }
        }
        if (input != "") {
            this->ruleList.push_back(Rule(input, output));
        }
    }
}
