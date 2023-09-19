#include "water_sim.hpp"
#include <glm/gtc/matrix_transform.hpp>
//Placeholder
#include "cgra/cgra_wavefront.hpp"

using namespace glm;

water_sim::water_sim(GLuint shader){
	m_mesh = cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//simple_water.obj")).build();
	m_shader = shader;
	m_color = vec3(0.3,0.5,0.7);
}

void water_sim::draw(const mat4 &view, const mat4 &proj) {
	// set up the shader for every draw call
    mat4 modelview = view * translate(mat4(1), vec3(0,-1,0));
	glUseProgram(m_shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
	glUniform3fv(glGetUniformLocation(m_shader, "uColor"), 1, value_ptr(m_color));
	m_mesh.draw();
}  
