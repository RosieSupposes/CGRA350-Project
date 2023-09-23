#pragma once

// glm
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "basic_model.hpp"
#include "collider.hpp"

class terrain{
private:
    
    basic_model m_model;
    
public:
    
    collider m_collider;
    terrain();
    terrain(GLuint shader, std::string file);
    terrain(GLuint shader, std::string file, glm::mat4 transform);
	void draw(const glm::mat4 &view, const glm::mat4 &proj);
    
}; 
