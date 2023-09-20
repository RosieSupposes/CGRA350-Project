#include "water_sim.hpp"
#include <glm/gtc/matrix_transform.hpp>
//Placeholder
#include "cgra/cgra_wavefront.hpp"

using namespace glm;

water_sim::water_sim(){
	m_mesh = cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//simple_water.obj")).build();
	m_color = vec3(0.3,0.5,0.7);
}

void water_sim::simulate(){
	
}

void water_sim::reload(){
	
}

void water_sim::draw(const mat4 &view, const mat4 &proj, GLuint shader) {
	// set up the shader for every draw call
    mat4 modelview = view * translate(mat4(1), vec3(0,-1,0));
	glUseProgram(shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
	glUniform3fv(glGetUniformLocation(shader, "uColor"), 1, value_ptr(m_color));
	m_mesh.draw();
}  
