#pragma once

// glm
#include <string>
#include <glm/glm.hpp>

// project
#include "opengl.hpp"



class material{
private:
    
	GLuint m_texture = 0;
	glm::vec3 m_color;
	float m_metallic;
	float m_roughness;
    
public:
	GLuint m_shader = 0;
    material();
	material(GLuint shader);
	material(GLuint shader, GLuint texture);
	material(GLuint shader, GLuint texture, glm::vec3 color);
	material(GLuint shader, GLuint texture, glm::vec3 color, float metallic, float roughness);
	void load(const glm::mat4 &view, const glm::mat4 &proj);
}; 
