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
    glm::vec3 m_position{6,-25,-40};
    glm::vec3 m_velocity{0};
    float m_maxVelocity = 5;
    float m_velocityThreshold = 0.1;
    float m_maxDistanceFromOrigin;
    
public:

    float m_pitch = 0.5;
	float m_yaw = 0;

    camera();
    camera(glm::vec3 position);
    void updateProjection(int frameWidth, int frameHeight);
    void setPosition(glm::vec3 position);
    void move(glm::vec3 displacement);
    void reduceVelocity();
    void update();
    glm::mat4 getProjection();
    glm::mat4 getView();
}; 
