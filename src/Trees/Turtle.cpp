#include "Turtle.hpp"

void Turtle::loadRules(vector<string> rules){
    for (int i = 0; i < (int)rules.size(); i++) {
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

Expression Turtle::parseExpression(string token){
    if (token.empty()) {
        return Expression(); // Empty input
    }

    // Handle brackets
    if (token[0] == '(' && token[token.size() - 1] == ')') {
        int openCount = 0;
        int closeCount = 0;
        bool foundMatching = false;

        for (int i = 1; i < (int)token.size() - 1; i++) {
            if (token[i] == '(') {
                openCount++;
            } else if (token[i] == ')') {
                closeCount++;
            }

            if (openCount == closeCount) {
                foundMatching = true;
                return Expression(parseExpression(token.substr(1, i - 1)), token[i + 1], parseExpression(token.substr(i + 2, token.size() - i - 2)));
            }
        }

        if (!foundMatching) {
            return Expression(); // Mismatched brackets
        }
    }

    // Handle operators
    char operators[] = {'+', '-', '*', '/', '>'};
    for (char op : operators) {
        int pos = (int)token.find(op);
        if (pos != (int)string::npos) {
            return Expression(parseExpression(token.substr(0, pos)), op, parseExpression(token.substr(pos + 1)));
        }
    }

    // Handle constants
    if (token[0] == 'd') {
        return Expression(distance);
    }
    if (token[0] == 'a') {
        return Expression(angle);
    }
    if (token[0] == 'l') {
        return Expression(level);
    }
    if (token[0] == 'r') {
        return Expression(radius);
    }

    // Handle numeric values
    try {
        return Expression(stof(token)); // must be a float
    } catch (const std::invalid_argument& e) {
        return Expression(); // Invalid numeric input
    }
}

string Turtle::getCommand(string axiom, int depth){
    string command = axiom;
    for(int i = 0; i < depth; i++){
        for (Rule rule : ruleList) {
            command = rule.applyRule(command);
        }
    }
    return command; //currently only applies one rule
}
