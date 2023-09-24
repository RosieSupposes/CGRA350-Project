#pragma once

//std
#include <vector>
#include <iostream>
#include <string>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"

struct Rule {
    std::string input;
    std::string output;

    Rule(std::string input,std::string output);
    ~Rule();

    std::string applyRule(std::string command);
};
