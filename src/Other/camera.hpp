#pragma once

// glm
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"

class camera{
private:
    glm::mat4 m_view{1};
    glm::mat4 m_proj{1};
    glm::vec3 m_position{-25,-30,-25};
    
public:

    float m_pitch = .86;
	float m_yaw = -.86;

    camera();
    camera(glm::vec3 position);
    void updateProjection(int frameWidth, int frameHeight);
    void setPosition(glm::vec3 position);
    void move(glm::vec3 displacement);
    void update();
    glm::mat4 getProjection();
    glm::mat4 getView();
}; 
