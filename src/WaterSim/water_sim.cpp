#include "water_sim.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "cgra/cgra_wavefront.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include <cstdlib> 
#include <ppl.h>

using namespace std;
using namespace glm;
using namespace cgra;

water_sim::water_sim(bool* enabled) {
	water_sim::enabled = enabled;
	reload();
}

void water_sim::simulate(){
	//find_neighbours();

	for (int i = 0; i < (int)particles.size(); i++){
		// Calculate density
		calculate_pressure_density(i);
		particles[i].velocity += vec3(0.0f, -gravity, 0.0f) * timestep;
	}

	for (int i = 0; i < (int)particles.size(); i++){
		// Calculate pressure force
		vec3 pressure_force = calculate_pressure_force(i);
		particles[i].velocity += (pressure_force / particles[i].density) * timestep;
	}

	for (int i = 0; i < (int)particles.size(); i++){
		// Update velocity
		particles[i].position += particles[i].velocity * timestep;


		// Check for collisions
		vec3 half_bounds = bounds_size * 0.5f;
		if (abs(particles[i].position.x) > half_bounds.x) {
        	particles[i].position.x = half_bounds.x * sign(particles[i].position.x);
        	particles[i].velocity.x *= -1.0f * bound_dampening;
    	}
		if (abs(particles[i].position.y) > half_bounds.y) {
			particles[i].position.y = half_bounds.y * sign(particles[i].position.y);
			particles[i].velocity.y *= -1.0f * bound_dampening;
		}
		if (abs(particles[i].position.z) > half_bounds.z) {
			particles[i].position.z = half_bounds.z * sign(particles[i].position.z);
			particles[i].velocity.z *= -1.0f * bound_dampening;
		}
	}
}

void water_sim::reload(){
	particles.clear();
	generate_particles();
}

void water_sim::draw(const mat4 &view, const mat4 &proj, material &material) {
	for (int i = 0; i < (int)particles.size(); i++){
		particles[i].draw(view, proj, material);
	}
	Particle p;
	p.position = vec3(0,0,0);
	p.draw(view, proj, material);
}

void water_sim::renderGUI(int height, int pos){
	ImGui::SetNextWindowPos(ImVec2(5.0f, (float)pos), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300.0f, (float)height), ImGuiSetCond_Once);
	ImGui::Begin("Water", 0);
	ImGui::Text("Water");
	ImGui::Checkbox("Enabled", enabled);
	if (ImGui::Button("Reload")) reload();
	ImGui::SliderFloat("Timestep", &timestep, 0.0f, 1.0f);
	ImGui::SliderFloat3("Bounds Size", value_ptr(bounds_size), 0.0f, 100.0f);
	ImGui::SliderFloat("Bounds Dampening", &bound_dampening, 0.0f, 1.0f);
	ImGui::SliderInt("Particle Count", &particle_count, 0, 100000);
	ImGui::SliderFloat("Particle Spacing", &particle_spacing, 0.0f, 100.0f);
	ImGui::Text("Water Properties");
	ImGui::SliderFloat("Smoothing Radius", &smoothing_radius, 0.0f, 100.0f);
	ImGui::SliderFloat("Mass", &mass, 0.0f, 100.0f);
	ImGui::SliderFloat("Target Density", &target_density, 0.0f, 100.0f);
	ImGui::SliderFloat("Pressure Multiplier", &pressure_multiplier, 0.0f, 100.0f);
	ImGui::SliderFloat("Gravity", &gravity, 0.0f, 100.0f);

	ImGui::End();
}

void water_sim::generate_particles(){
	int grid_size = (int)ceil(pow(particle_count, 1.0f / 3.0f));
	float spacing = 1.0f / (float)grid_size * particle_spacing;
	for (int x = 0; x < grid_size; x++){
		for (int y = 0; y < grid_size; y++){
			for (int z = 0; z < grid_size; z++){
				if ((int)particles.size() < particle_count){
					Particle p;
					float rand_x = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * spacing;
                    float rand_y = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * spacing;
                    float rand_z = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * spacing;
					p.position = vec3(x, y, z) * spacing + vec3(rand_x, rand_y, rand_z);
					particles.push_back(p);
				}
			}
		}
	}

	// Calculate the average position of all the particles.
	float average_x = 0.0f;
	float average_z = 0.0f;
	float average_y = 0.0f;

	for (int i = 0; i < (int)particles.size(); i++) {
		average_x += particles[i].position.x;
		average_z += particles[i].position.z;
		average_y += particles[i].position.y;
	}

	average_x /= particles.size();
	average_z /= particles.size();
	average_y /= particles.size();

	// Center the particles.
	for (int i = 0; i < (int)particles.size(); i++) {
		particles[i].position.x -= average_x;
		particles[i].position.z -= average_z;
		particles[i].position.y -= average_y;
	}
}

float water_sim::smoothing_kernel(float dist){
	if (dist >= smoothing_radius) return 0.0f;

	float volume = (PI * pow(smoothing_radius, 4.0f)) / 6;
	return (smoothing_radius - dist) * (smoothing_radius - dist) / volume;
}

float water_sim::smoothing_kernel_derivative(float dist){
	if (dist >= smoothing_radius) return 0.0f;
	float scale = 12 / (PI * pow(smoothing_radius, 4.0f));
	return (dist - smoothing_radius) * scale;
}

void water_sim::calculate_pressure_density(int n){
	float density = 0.0f;
	for (int j = 0; j < (int)particles.size(); j++){
		vec3 pos2 = particles[j].position;
		float dist = length(particles[n].position - pos2);
		float influence = smoothing_kernel(dist);
		density += mass * influence;
	}
	particles[n].density = density;
	particles[n].pressure = pressure_multiplier * (density - target_density);
}

vec3 water_sim::calculate_pressure_force(int n){
	vec3 force = vec3(0.0f, 0.0f, 0.0f);
	for (int j = 0; j < (int)particles.size(); j++){
		vec3 offset = particles[n].position - particles[j].position;
		float dist = length(offset);
		vec3 dir = dist == 0 ? random_dir() : normalize(offset);
		float influence = smoothing_kernel(dist);
		float shared_pressure = (particles[n].pressure + particles[j].pressure) / 2;
		force += shared_pressure * dir * influence * mass / particles[j].density;
	}
	return force;		
}

vec3 water_sim::random_dir(){
	float x = (float)rand() / (float)RAND_MAX;
	float y = (float)rand() / (float)RAND_MAX;
	float z = (float)rand() / (float)RAND_MAX;
	return normalize(vec3(x, y, z));
}

void water_sim::find_neighbours(){
	concurrency::parallel_for(0, (int)particles.size(), [&](int i){
		particles[i].neighbours.clear();
		for (int j = 0; j < (int)particles.size(); j++){
			if (i == j) continue;
			float dist = length(particles[i].position - particles[j].position);
			if (dist < smoothing_radius * 2){
				particles[i].neighbours.push_back(&particles[j]);
			}
		}
	});
}
