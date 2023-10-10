#pragma once

// glm
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "basic_model.hpp"
#include "collider.hpp"
#include "cgra/cgra_mesh.hpp"
#include "material.hpp"

class terrain{
private:
    basic_model m_model;
    collider m_collider;
    
public:
    terrain();
    terrain(std::string file);
    terrain(std::string file, glm::mat4 transform);
    bool doesCollide(Ray ray);
    Collision checkCollision(Ray ray);
	void draw(const glm::mat4 &view, const glm::mat4 &proj, material material);
}; 
