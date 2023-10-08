#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "water_particle.hpp"
#include "Other/material.hpp"


class water_sim {
private:
	std::vector<Particle> particles;
    GLuint water_shader;


public:
    water_sim();
    water_sim(bool* enabled); 

    void reload();
    void simulate();
    void renderGUI(int height, int pos);
    void manage_boundry(glm::vec3 &vel, glm::vec3 &position);
    Particle* create_particle(glm::vec3 position = glm::vec3(0,0,0));

    // Fluid Calculations
    void calculate_density(Particle &particle);
    void calculate_pressure(Particle &particle);

    // Force Calculations
    glm::vec3 calculate_forces(Particle &particle);

    float poly6_kernel(float r, float h);
    float spiky_kernel(float r, float h);
    float spiky_kernel2(float r, float h);
    glm::vec3 spiky_gradient(float r, glm::vec3 dir);


	void draw(const glm::mat4 &view, const glm::mat4 &proj, material &material);
    void draw_boundary(const glm::mat4 &view, const glm::mat4 &proj, material &material);

    // Settings
    glm::vec3 bottomLeft = glm::vec3(-100, 0, -100);
    glm::vec3 topRight = glm::vec3(100, 50, 100);

    // FLuid Constants
    float restDensity = 2.0f;
    float gasConstant = 5.0f;
    float viscosity = 0.3f;
    float particleMass = 1.0f;
    float smoothingRadius = 1.17f;
    float timestep = 0.02f;
    float boundDamping = -0.3f;

    bool* enabled;

    int count = 0;
};  


