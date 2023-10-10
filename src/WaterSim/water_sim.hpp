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
    float PI = 3.14159265359f;


public:
    water_sim(){};
    water_sim(bool* enabled); 

    void reload();
    void simulate();
    void renderGUI(int height, int pos);
    void generate_particles();

    float smoothing_kernel(float dist);
    float smoothing_kernel_derivative(float dist);
    void calculate_pressure_density(int n);
    glm::vec3 random_dir();
    glm::vec3 calculate_pressure_force(int n);

    void find_neighbours();

	void draw(const glm::mat4 &view, const glm::mat4 &proj, material &material);

    bool* enabled;
    float timestep = 0.01f;
    float bound_dampening = 0.85f;
    int particle_count = 402;
    float particle_spacing = 11.0f;
    glm::vec3 bounds_size = glm::vec3(100.0f, 100.0f, 100.0f);

    // Water properties
    float smoothing_radius = 1.2f;
    float mass = 1.0f;
    float target_density = 2.75f;
    float pressure_multiplier = 0.5f;
    float gravity = 9.8f;

};  


