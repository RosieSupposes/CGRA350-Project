#pragma once

// glm
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "basic_model.hpp"
#include "material.hpp"

class terrain{
private:
    basic_model m_model;
    glm::vec3 m_scale{8};
    
    
public:
    terrain();
    terrain(std::string file);
	void draw(const glm::mat4 &view, const glm::mat4 &proj, material material);
}; 