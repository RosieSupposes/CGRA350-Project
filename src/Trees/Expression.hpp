#pragma once

//std
#include <random>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"

struct Expression {
    std::shared_ptr<Expression> exp1;
    std::shared_ptr<Expression> exp2;
    char op;
    float number;
    int constructor;

    Expression(Expression exp1, char op,  Expression exp2);
    Expression(float number);
    Expression();
    
    std::string toString();
    float evaluate(std::mt19937 &randomNumberGenerator);
    
};
