#pragma once

// glm
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "material.hpp"

struct Ray{
    glm::vec3 point;
    glm::vec3 direction;
    float length;
};

class collider{
private:
    std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
    void loadPoints(cgra::mesh_builder mesh_builder);
    
public:
    collider();
    collider(std::string file);
    collider(cgra::mesh_builder mesh_builder);
    bool checkIntersection(Ray ray);
    vec3 intersectionPoint(Ray ray);
}; 
