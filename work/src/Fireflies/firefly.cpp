#include "firefly.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"

using namespace glm;

firefly::firefly(glm::mat4 transform){
    static cgra::gl_mesh sharedMesh = cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//placeholderSphere.obj")).build();
	m_mesh = sharedMesh;
    m_color = glm::vec3(0.8,0.7,0.2);
    m_modelTransform = transform;
	//m_texture;
}

void firefly::draw(const mat4 &view, const mat4 &proj, GLuint shader) {
	// set up the shader for every draw call
    mat4 modelview = view * m_modelTransform;
	glUseProgram(shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(m_color));
	m_mesh.draw();
}  
