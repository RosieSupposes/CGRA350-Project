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
    
    glm::vec3 m_velocity{0};
    glm::vec3 minBounds{-50,-40,-50};
    glm::vec3 maxBounds{50,-2,50};
    float m_maxVelocity = 5;
    float m_velocityThreshold = 0.1;
    float m_maxDistanceFromOrigin;
    
public:

    float m_pitch = 0.5;
	float m_yaw = 0;

    camera();
    camera(glm::vec3 position);
    void updateProjection(int frameWidth, int frameHeight);
    glm::vec3 m_position{6,-25,-40};
    void move(glm::vec3 displacement);
    void reduceVelocity();
    void update();
    void setInBounds();
    glm::mat4 getProjection();
    glm::mat4 getView();
}; 
