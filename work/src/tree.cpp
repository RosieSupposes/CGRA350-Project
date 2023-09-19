#include "tree.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

tree::tree(GLuint shader, glm::mat4 transform){
    static cgra::gl_mesh sharedMesh = cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//teapot.obj")).build();
	m_shader = shader;
	m_modelTransform = transform * scale(mat4(1), vec3(0.1,1,0.1));
	m_mesh = sharedMesh;
}

void tree::draw(const mat4 &view, const mat4 &proj) {
	// set up the shader for every draw call
    mat4 modelview = view * m_modelTransform;
	glUseProgram(m_shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
	glUniform3fv(glGetUniformLocation(m_shader, "uColor"), 1, value_ptr(m_color));
	m_mesh.draw();
}  
 
