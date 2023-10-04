#include "basic_model.hpp"
#include <glm/gtc/matrix_transform.hpp>

//Placeholder
#include "cgra/cgra_wavefront.hpp"

using namespace glm;

basic_model::basic_model(){
	
}

basic_model::basic_model(GLuint shader, std::string file){
	m_shader = shader;
	m_mesh = cgra::load_wavefront_data(file).build();
}

basic_model::basic_model(GLuint shader, std::string file, vec3 colour){
	m_shader = shader;
	m_mesh = cgra::load_wavefront_data(file).build();
	m_color = colour;
}

basic_model::basic_model(GLuint shader, std::string file, mat4 transform){
	m_modelTransform = transform;
	m_shader = shader;
	m_mesh = cgra::load_wavefront_data(file).build();
}

basic_model::basic_model(GLuint shader, std::string file, mat4 transform, vec3 colour){
	m_modelTransform = transform;
	m_shader = shader;
	m_mesh = cgra::load_wavefront_data(file).build();
	m_color = colour;
}

void basic_model::draw(const mat4 &view, const mat4 &proj) {
	// set up the shader for every draw call
    mat4 modelview = view * m_modelTransform;
	glUseProgram(m_shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
	glUniform3fv(glGetUniformLocation(m_shader, "uColor"), 1, value_ptr(m_color));
	m_mesh.draw();
} 
