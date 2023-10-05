#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

// project
#include "opengl.hpp"
#include "Rule.hpp"
#include "Expression.hpp"


using namespace std;
using namespace glm;
using namespace cgra;

/*
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

struct Vertex {
    vector<int> connections;
    vector<bool> used;
    int id;
    int branch;
    vec3 pos;
};

class Circle {
public:
    vector<Vertex> points;
    vec3 center;
    vec3 rotation;
    mat4 matrixRotation = mat4(1);
    float radius;
    
    Circle() {};
    Circle(vec3 center, float radius, int resolution, mat4 rotation);
    Circle(vec3 center, float radius, vec3 rotation);
    
    void addPoint(float angle);
    Vertex getPoint(float angle);
    vector<Vertex> getPoints();
};

class Branch {
public:
    Circle base;
    vector<Branch> branches;
    
    Branch() {};
    Branch(Circle base);
};

struct Point {
    vec3 position;
    float angleH;
    float angleL;
    float angleU;
    mat4 rotation;
    float distance;
    float angle;
    float radius;
    shared_ptr<Branch> branch; //circles (sub-cylinders) make up branches
    shared_ptr<Circle> circle; //pointer for generating branches
};

class Turtle {
    vector<Rule> ruleList;
    
    //fields to help with mesh generation
    float distance = 1;
    float angle = pi<float>()/8;
    float radius = 0.1;
    int level = 0;
    vector<Point> stack; //stack to handle brackets
    mat4 rotation = mat4(1);
    vec3 position = vec3(0,0,0);
    float angleH = 0;
    float angleL = 0;
    float angleU = 0;
    bool moved = true;
    int id = 0;
    std::shared_ptr<Circle> circle;
    std::shared_ptr<Circle> lastCircle;
    
public:
    void loadRules(vector<string> rules); //populates ruleList
    Expression parseExpression(string token); //generates an EXP
    string getCommand(string axiom, int depth); //get commands (outputs) of rules
    vector<gl_mesh> generateMesh(string command, int resolution, mt19937 randomNumberGenerator); //generate mesh
    gl_mesh cylinder(Circle base, Circle branch); //generate a cyliner mesh
};
