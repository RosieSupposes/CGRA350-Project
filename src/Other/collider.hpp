#pragma once

// glm
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "material.hpp"
#include "basic_model.hpp"

struct Ray{
    glm::vec3 point;
    glm::vec3 direction;
    float length = 3.4e+38;
};

struct Collision
{
    bool hit;
    glm::vec3 point;
    glm::vec3 normal;
};

class collider{
private:
    std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
    void loadPoints(cgra::mesh_builder mesh_builder, glm::mat4 transform);
    
public:
    collider();
    collider(std::string file);
    collider(std::string file, glm::mat4 transform);
    collider(cgra::mesh_builder mesh_builder);
    collider(cgra::mesh_builder mesh_builder, glm::mat4 transform);
    bool doesCollide(Ray ray);
    Collision checkCollision(Ray ray);
}; 

// struct BouncyBall{
//     basic_model model;
//     glm::vec3 position;
//     collider col;
//     glm::vec3 velocity;
// };
