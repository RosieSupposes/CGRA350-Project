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

	glm::mat4 m_modelTransform{ 1.0 };

	glm::mat4 m_scale;


public:
	float brightness;
	glm::vec3 pos;
	float search_precision;     //guassian distribution, uniform distribution, or distrubuted random number vector

	firefly(glm::mat4 transform);
	void draw(const glm::mat4& view, const glm::mat4& proj, GLuint shader);
};
