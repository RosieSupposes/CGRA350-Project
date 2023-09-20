#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"

class simple_water {
private:
    GLuint m_shader = 0;
	cgra::gl_mesh m_mesh;
    glm::vec3 m_color = glm::vec3(0,0.2f,0.7f);
    glm::mat4 m_modelTransform{1.0};
	GLuint m_texture;
    
    
public:
    simple_water(GLuint shader);
	void draw(const glm::mat4 &view, const glm::mat4 &proj);
}; 
