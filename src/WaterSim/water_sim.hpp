#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "water_particle.hpp"


class water_sim {
private:
	std::vector<Particle> particles;


public:
    water_sim();
    void reload();
    void simulate();
    void manageBoundry(glm::vec3 &vel, glm::vec3 &position);
    void create_particle(glm::vec3 position = glm::vec3(0,0,0));
	void draw(const glm::mat4 &view, const glm::mat4 &proj, GLuint shader);

    // Settings
    glm::vec3 bottomLeft = glm::vec3(-8,-8,-8);
    glm::vec3 topRight = glm::vec3(8,8,8);

    // FLuid Constants
    float boundDamping = -0.3f;
    float restDensity = 1.0f;
    float gasConstant = 2.0f;
    float viscosity = -0.003f;
    float particleMass = 1.0f;
    float timestep = 0.001f;
};  
