#pragma once

//std
#include <random>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "Turtle.hpp"

using namespace std;
using namespace glm;

struct cylinder_model {
    //GLuint shader = 0;
    cgra::gl_mesh mesh;
    vec3 color = vec3(0.4196, 0.2863, 0.1686);
    mat4 modelTransform{1.0};
    GLuint texture;
    
    void draw(const mat4 &view, const mat4 proj, GLuint shader){
        mat4 modelview = view * modelTransform;
        
        glUseProgram(shader); // load shader and variables
        glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
        glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(color));
        mesh.draw(); // draw
    }
};

class Turtle;

class tree {
private:
    cylinder_model model;
    vector<cylinder_model> cylinder_models;
    vector<string> rules;
    
    string axiom;
    //int depth = 3;
    int resolution = 3;
    
    mt19937 randomNumberGenerator;
    //unsigned int seed;
    //random_device rd;

public:
    tree(glm::mat4 transform, int recursion_depth, string style);
	void draw(const glm::mat4 &view, const glm::mat4 &proj, GLuint shader);
};  
 
