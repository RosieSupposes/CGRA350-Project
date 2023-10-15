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
    bool multithread = false;


public:
    water_sim(){};
    water_sim(bool* enabled); 

    void reload();
    void simulate();
    void renderGUI(int height, int pos);
    void generate_particles();

    float smoothing_kernel(float dist);
    float smoothing_kernel_derivative(float dist);
    float smoothing_kernel_second_derivative(float dist);
    void calculate_pressure_density(int n);

    glm::vec3 random_dir();
    glm::vec3 calculate_force(int n);

    void find_neighbours();

	void draw(const glm::mat4 &view, const glm::mat4 &proj, material &material);

    bool* enabled;
    float timestep = 23.7f;
    float bound_dampening = 0.22f;
    int particle_count = 380;

    glm::vec3 top_left = glm::vec3(-5.0f, 100.0f, -5.0f);
    glm::vec3 bottom_right = glm::vec3(5.0f, -3.0f, 5.0f); 
    glm::vec3 spawn_pos = glm::vec3(0.0f, 0.0f, 0.0f);

    // Water properties
    float smoothing_radius = 0.85f;
    float mass = 1.0f;
    float target_density = 1.7f;
    float pressure_multiplier = 12.06f;
    float gravity = 3.0f;
    float viscosity = 2.0f;

    glm::vec3 particle_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    int count = 0;
    bool running = false;
    float prev_time = 0.0f;

};  


