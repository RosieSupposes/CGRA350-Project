#include "tree.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

tree::tree(glm::mat4 transform){
    //static cgra::gl_mesh sharedMesh = cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//teapot.obj")).build();
    //m_modelTransform = transform * scale(mat4(1), vec3(0.1,1,0.1));
    //m_mesh = sharedMesh;
    
    //placeholder
    static cgra::gl_mesh placeholderMesh = cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//teapot.obj")).build();
    cylinder_model placeholder = cylinder_model();
    placeholder.modelTransform = transform * scale(mat4(1), vec3(0.1,1,0.1));
    //placeholder.shader = shader;
    placeholder.mesh = placeholderMesh;
    cylinders.push_back(placeholder);
}

void tree::draw(const mat4 &view, const mat4 &proj, GLuint shader) {
    for (int i = 0; i < (int)cylinders.size(); i++) {
        cylinders.at(i).draw(view, proj, shader);
    }
}  
 
