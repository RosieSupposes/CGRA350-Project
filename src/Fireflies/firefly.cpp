#include "firefly.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

firefly::firefly(glm::vec3 p, float b, glm::vec3 sp){
    static cgra::gl_mesh sharedMesh = cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//placeholderSphere.obj")).build();
	m_mesh = sharedMesh;
    m_color = glm::vec3(0.8,0.7,0.2);
    

	pos = p;
	brightness = b;
	search_precision = sp;
	//m_texture;
}

void firefly::draw(const mat4 &view, const mat4 &proj, GLuint shader) {
	// set up the shader for every draw call
	glUseProgram(shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));

	
	mat4 modelview = translate(view, pos);
	modelview = scale(modelview, vec3(0.3));
	m_color = vec3(brightness);
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(m_color));
	m_mesh.draw();
}

void firefly::setPos(glm::vec3 p){
	this->pos = p;
}
