#include "tree.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;
using namespace cgra;

tree::tree(glm::mat4 transform, int recursion_depth, string style){
    vector<Rule> rules;
        
    //tree base
    Circle base(vec3(0,0,0), 1, vec3(0,0,0));
    
    //tree branches
    vector<Circle> branches;
    branches.push_back(Circle(vec3(0, 10, -1), 1, vec3(1, 0, 1)));
    branches.push_back(Circle(vec3(0, 5, 4), 1, vec3(pi<float>() / 2, 0, 0)));
    
    //turtle to translate l-system to trees
    Turtle turtle = Turtle();
    
    //set rules for different styles of trees
    if(style == "Basic"){
        //clamp basic tree at depth of 4
        recursion_depth = std::min(recursion_depth, 4);
        axiom = "EF";
        this->rules.push_back("EF:F[C+EF][C-EF][C^EF][C&EF][C*EF]");
        this->rules.push_back("C:A(1)D(d*(1/2))R((1/13)/(2*l))");
    } else{
        //clamp complex tree at depth of 7
        recursion_depth = std::min(recursion_depth, 7);
        axiom = "R((1/4))D(0.1)FD(0.3)R((4/5)*r)FD(0.7)R((11/12)*r)FD(2>3)R((10/11)*r)FC";
        this->rules.push_back("C:P(0.1)F[PFC][B]");
        this->rules.push_back("P:A(0.1)+A(0.1)^D((d/6)/(d/2))R(0.8*r)");
        this->rules.push_back("B:A(1)/A(0.8)+D(6/(l*4))R(0.6*r)FC");
    }
    
    //convert rules to rule objects
    turtle.loadRules(this->rules);
    
    //enable random generation
    //seed = rd();
    //randomNumberGenerator = mt19937(seed);
    randomNumberGenerator = mt19937(0); //dummy seed for now

    //turtle to make the tree mesh - populate cylinder models
    string command = turtle.getCommand(axiom, recursion_depth);
    vector<gl_mesh> cylinders = turtle.generateMesh(command, resolution, randomNumberGenerator);
    for (int i = 0; i < (int)cylinders.size(); i++) {
        cylinder_model model = cylinder_model();
        if(style == "Basic"){
            model.modelTransform = transform * scale(mat4(1), vec3(6));
        } else{
            model.modelTransform = transform * scale(mat4(1), vec3(2));
        }
        //model.shader = shader;
        model.mesh = cylinders.at(i);
        cylinder_models.push_back(model);
    }
    
}

void tree::draw(const mat4 &view, const mat4 &proj, GLuint shader) {
    for (int i = 0; i < (int)cylinder_models.size(); i++) {
        cylinder_models.at(i).draw(translate(view, vec3(0, 0.5, 0)), proj, shader);
    }
}  
 
