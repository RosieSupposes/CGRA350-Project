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
	float delta_time = glfwGetTime() - prev_time;
	count++;
	if (count % 10 == 0 && running){
		// add randomness
		float x_rand = rand() / (float)INT_MAX;
		float y_rand = rand() / (float)INT_MAX;
		float z_rand = rand() / (float)INT_MAX;

		Particle p;
		p.position = vec3(x_rand, y_rand, z_rand);
		p.estimated_position = vec3(x_rand, y_rand, z_rand);
		particles.push_back(p);

	}
	concurrency::parallel_for(0, (int)particles.size(), [&](int i){
		// Update estimated position
		particles[i].estimated_position = particles[i].position + particles[i].velocity * delta_time * timestep;
		// Calculate density
		calculate_pressure_density(i);
	});


	concurrency::parallel_for(0, (int)particles.size(), [&](int i){
		// Update velocity
		vec3 total_force = calculate_force(i);
		particles[i].velocity += (total_force / particles[i].density) * delta_time * timestep;
		particles[i].position += particles[i].velocity * delta_time * timestep;


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
	});
	prev_time = glfwGetTime();
}

void water_sim::reload(){
	particles.clear();
	//generate_particles();
}

void water_sim::draw(const mat4 &view, const mat4 &proj, material &material) {
	for (int i = 0; i < (int)particles.size(); i++){
		particles[i].draw(view, proj, material);
	}
}

void water_sim::renderGUI(int height, int pos){
	ImGui::SetNextWindowPos(ImVec2(5.0f, (float)pos), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300.0f, (float)height), ImGuiSetCond_Once);
	ImGui::Begin("Water", 0);
	ImGui::Text("Water");
	ImGui::Checkbox("Enabled", enabled);
	if (ImGui::Button("Reload")) reload();
	ImGui::Checkbox("Running", &running);
	ImGui::SliderFloat("Timestep", &timestep, 0.0f, 100.0f);
	ImGui::SliderFloat3("Bounds Size", value_ptr(bounds_size), 0.0f, 100.0f);
	ImGui::SliderFloat("Bounds Dampening", &bound_dampening, 0.0f, 1.0f);
	ImGui::SliderInt("Particle Count", &particle_count, 0, 100000);
	ImGui::SliderFloat("Particle Spacing", &particle_spacing, 0.0f, 100.0f);
	ImGui::Text("Water Properties");
	ImGui::SliderFloat("Smoothing Radius", &smoothing_radius, 0.0f, 100.0f);
	ImGui::SliderFloat("Mass", &mass, 0.0f, 100.0f);
	ImGui::SliderFloat("Target Density", &target_density, 0.0f, 100.0f);
	ImGui::SliderFloat("Pressure Multiplier", &pressure_multiplier, 0.0f, 100.0f);
	ImGui::SliderFloat("Viscosity", &viscosity, 0.0f, 100.0f);
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
					p.position = vec3(x, y, z) * spacing;
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

	float rand_x = rand() / (float)INT_MAX;
	float rand_y = rand() / (float)INT_MAX;
	float rand_z = rand() / (float)INT_MAX;

	// Center the particles.
	for (int i = 0; i < (int)particles.size(); i++) {
		particles[i].position.x -= average_x + rand_x;
		particles[i].position.z -= average_z + rand_z;
		particles[i].position.y -= average_y + rand_y;
	}
	
}

float water_sim::smoothing_kernel(float dist){
	// poly6
	const float Poly6_constant = (315.0f / (64.0f * PI * (float)pow(smoothing_radius, 9.0f)));
	if (dist > smoothing_radius) return  0.0f;
	return Poly6_constant * pow(smoothing_radius * smoothing_radius - dist * dist, 3);
}

float water_sim::smoothing_kernel_derivative(float dist){
	// spiky
	const float Spiky_constant = (-45.0f / (PI * (float)pow(smoothing_radius, 6.0f)));
	if (dist > smoothing_radius) return 0.0f;
	return Spiky_constant * pow(smoothing_radius - dist, 2);
}

void water_sim::calculate_pressure_density(int n){
	vec3 pos = particles[n].estimated_position;
	float density = 0.0f;
	concurrency::parallel_for(0, (int)particles.size(), [&](int j){
		vec3 diff = particles[j].position - pos;
		float dist = dot(diff, diff);
		dist = sqrt(dist);
		if (smoothing_radius * smoothing_radius > dist){
			density += mass * smoothing_kernel(dist * 0.004);
		}
	});
	particles[n].density = std::max(density, target_density);
	particles[n].pressure = pressure_multiplier * (density - target_density);
}

vec3 water_sim::calculate_force(int n){
	vec3 pressure = vec3(0.0f, 0.0f, 0.0f);
	vec3 viscosity_force = vec3(0.0f, 0.0f, 0.0f);
	for (int j = 0; j < (int)particles.size(); j++){
		if (j != n) {
			vec3 diff = particles[j].position - particles[n].estimated_position;
			float dist = dot(diff, diff);
			dist = sqrt(dist);
			if (dist > 0 && dist < smoothing_radius){
				vec3 dir = normalize(particles[n].estimated_position - particles[j].position);
				pressure += (particles[j].pressure + particles[n].pressure) / (2.0f * particles[j].density * particles[n].density) * smoothing_kernel_derivative(dist) * dir;
				float w = -(pow(dist, 3.0) / (2 * pow(smoothing_radius, 3.0f))) + (pow(dist, 2.0) / pow(smoothing_radius, 2.0f)) + (smoothing_radius / (2 * dist)) - 1;
				viscosity_force += viscosity * (1.0f / particles[j].density) * (particles[j].velocity - particles[n].velocity) * dir * w;
			}
		}
	}
	return -pressure + vec3(0, -gravity, 0) + viscosity_force;		
}
