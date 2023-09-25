#include "Rule.hpp"
#include "Expression.hpp"

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"

using namespace std;
using namespace glm;

/**
 Turtle Language:
 F: Move forward and draw a cylinder
 f: Move forward without drawing a cylinder
 +: Turn right
 -: Turn Left
 A: Pitch up
 &: Pitch down
 /: Roll right
 \: Roll left
 [: Save state
 ]: Load last saved state

 D(EXP): Set distance to expression EXP
 A(EXP): Set angle to expression EXP
 R(EXP): Set radius of cylinder to expression EXP

 EXP: expression supporting + - * / ()

 Special variables:
 EXP1>EXP2: A random number from EXP1 to EXP2
 d: The current distance moved
 a: The current change in angle
 r: The current radius
 1: The current level (number of saves on the stack)
 */

class Turtle {
    vector<Rule> ruleList;
    float distance = 1;
    float angle = pi<float>()/8;
    float radius = 0.1;
    int level = 0;
    
public:
    void loadRules(vector<string> rules); //populates ruleList
    Expression parseExpression(string token); //generates an EXP
    string getCommand(string axiom, int depth); //get commands (outputs) of rules
};
