#pragma once

// glm
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"

class basic_model{
private:
    GLuint m_shader = 0;
	cgra::gl_mesh m_mesh;
    glm::vec3 m_color = glm::vec3(0.5);
    glm::mat4 m_modelTransform{1.0};
	GLuint m_texture;
    
    
public:
    basic_model();
    basic_model(GLuint shader, std::string file);
    basic_model(GLuint shader, std::string file, glm::vec3 colour);
    basic_model(GLuint shader, std::string file, glm::mat4 transform);
    basic_model(GLuint shader, std::string file, glm::mat4 transform, glm::vec3 colour);
	void draw(const glm::mat4 &view, const glm::mat4 &proj);
}; 
