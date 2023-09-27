#include "Expression.hpp"

using namespace std;

Expression::Expression(Expression exp1, char op,  Expression exp2) {
    this->exp1 = std::make_shared<Expression>(exp1);
    this->exp2 = std::make_shared<Expression>(exp2);
    this->op = op;
    constructor = 0;
}

Expression::Expression(float number) {
    this->number = number;
    constructor = 1;
}

Expression::Expression() {
    constructor = 2;
}

string Expression::toString() {
    if (constructor == 0) {
        return "(" +exp1->toString() + op + exp2->toString() + ")";
    }
    else if (constructor == 1) {
        return std::to_string(number);
    }
    else if (constructor == 2) {
        return "NAN";
    }
    return "NAN";
}

float Expression::evaluate(std::mt19937 &randomNumberGenerator) {
    if (constructor == 0) {
        if (exp1 != nullptr) {
            if (op == '+') {
                return exp1->evaluate(randomNumberGenerator) + exp2->evaluate(randomNumberGenerator);
            }
            else if (op == '-') {
                return exp1->evaluate(randomNumberGenerator) - exp2->evaluate(randomNumberGenerator);
            }
            else if (op == '*') {
                return exp1->evaluate(randomNumberGenerator) * exp2->evaluate(randomNumberGenerator);
            }
            else if (op == '/') {
                return exp1->evaluate(randomNumberGenerator) / exp2->evaluate(randomNumberGenerator);
            }
            else if (op == '>') {
                std::uniform_real_distribution<float> dis(exp1->evaluate(randomNumberGenerator), exp2->evaluate(randomNumberGenerator));
                return dis(randomNumberGenerator);
            }
        }
        else {
            return number;
        }
    }
    else if (constructor == 1) {
        return number;
    }
    else if (constructor == 2) {
        return NAN;
    }
    return NAN;
}
