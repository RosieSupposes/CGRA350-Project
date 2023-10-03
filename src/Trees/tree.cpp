#include "tree.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

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
    
    //basic test tree
    this->rules.push_back("EF:F[C+EF][C-EF][C^EF][C&EF]");
    this->rules.push_back("C:A(1)D(d*(1/2))R((1/10)/(2*l))");
    
    //convert rules to rule objects
    turtle.loadRules(this->rules);
    
    //enable random generation
    //seed = rd();
    //randomNumberGenerator = mt19937(seed);
    randomNumberGenerator = std::mt19937(0); //dummy seed for now

    //turtle to make the tree mesh - populate cylinder models
    string command = turtle.getCommand(axiom, depth);
    vector<cgra::gl_mesh> cylinders = turtle.generateMesh(command, resolution, randomNumberGenerator);
    for (int i = 0; i < (int)cylinders.size(); i++) {
        cylinder_model model = cylinder_model();
        model.modelTransform = transform * scale(mat4(1), vec3(6,6,6));
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
 
