#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"

class firefly {
private:
	GLuint m_shader = 0;
	GLuint m_texture;

	glm::vec3 m_color{ 0.7 };


	cgra::gl_mesh m_mesh;

	glm::mat4 m_scale;


public:
	float brightness;
	glm::vec3 pos;
	glm::vec3 velocity;
	std::vector<firefly*> neighbours;

	glm::vec3 search_precision;     //guassian distribution, uniform distribution, or distrubuted random number vector

	firefly(glm::vec3 pos, float brightness, glm::vec3 sp);

	void draw(const glm::mat4& view, const glm::mat4& proj, GLuint shader);
	void setPos(glm::vec3 p);
};
