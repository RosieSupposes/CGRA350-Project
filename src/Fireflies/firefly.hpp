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
	cgra::gl_mesh m_mesh;
    glm::vec3 m_color{0.7};
    glm::mat4 m_modelTransform{1.0};
	GLuint m_texture;

public:
    firefly(glm::mat4 transform);
	void draw(const glm::mat4 &view, const glm::mat4 &proj, GLuint shader);
};  
