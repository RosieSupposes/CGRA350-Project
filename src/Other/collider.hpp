#pragma once

// glm
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"

struct kd_node{
    //kd_node left;
    //kd_node right;
    bool isLeaf;
    glm::vec3 m_point;
    glm::vec3 findNearestPoint(glm::vec3 point);
};

class collider{
private:
    kd_node kd_tree;
    std::map<glm::vec3, std::vector<glm::vec3>> neighbours;
    
public:
    collider();
    collider(std::string file);
    glm::vec3 nearestPointOnCollider(glm::vec3 point);
}; 
