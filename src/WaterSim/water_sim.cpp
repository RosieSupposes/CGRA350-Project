#include "water_sim.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "cgra/cgra_wavefront.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include <cstdlib> 

using namespace std;
using namespace glm;
using namespace cgra;

water_sim::water_sim(bool* enabled) {
	water_sim::enabled = enabled;
	reload();
}

void water_sim::simulate(){
	float delta_time = (float)glfwGetTime() - prev_time;
	count++;
	if (count % 10 == 0 && running && particles.size() < particle_count){
		for (int x = 0; x < 3; x++){
			for (int y = 0; y < 3; y++){
				for (int z = 0; z < 3; z++){
					float x_rand = rand() % 2;
					float z_rand = rand() % 2;
					
					Particle p;
					p.position = vec3(x/2.0f, y/2.0f, z/2.0f) + spawn_pos + vec3(x_rand, -0.5f, z_rand);
					p.velocity = vec3(x_rand, -0.2f, z_rand);
					particles.push_back(p);
				}
			}
		}
	}

	for (int i = 0; i < (int)particles.size(); i++){
		// Update estimated position
		particles[i].estimated_position = particles[i].position + particles[i].velocity * delta_time * timestep;

		// Calculate density
		calculate_pressure_density(i);
	}


	for (int i = 0; i < (int)particles.size(); i++){
		// Update velocity
		vec3 total_force = calculate_force(i);
		particles[i].velocity += (total_force / particles[i].density) * delta_time * timestep;
		particles[i].position += particles[i].velocity * delta_time * timestep;


		// Check for collisions
		if (particles[i].position.x < top_left.x){
			particles[i].position.x = top_left.x;
			particles[i].velocity.x *= -bound_dampening;
		}
		if (particles[i].position.x > bottom_right.x){
			particles[i].position.x = bottom_right.x;
			particles[i].velocity.x *= -bound_dampening;
		}
		if (particles[i].position.y > top_left.y){
			particles[i].position.y = top_left.y;
			particles[i].velocity.y *= -bound_dampening;
		}
		if (particles[i].position.y < bottom_right.y){
			particles[i].position.y = bottom_right.y;
			particles[i].velocity.y *= -bound_dampening;
		}
		if (particles[i].position.z < top_left.z){
			particles[i].position.z = top_left.z;
			particles[i].velocity.z *= -bound_dampening;
		}
		if (particles[i].position.z > bottom_right.z){
			particles[i].position.z = bottom_right.z;
			particles[i].velocity.z *= -bound_dampening;
		}

	}
	prev_time = (float)glfwGetTime();
}

void water_sim::reload(){
	particles.clear();
}

void water_sim::draw(const mat4 &view, const mat4 &proj, material &material) {
	for (int i = 0; i < (int)particles.size(); i++){
		particles[i].draw(view, proj, material, particle_scale);
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
	ImGui::SliderFloat3("Bound Top Left", value_ptr(top_left), -100.0f, 100.0f);
	ImGui::SliderFloat3("Bound Bottom Right", value_ptr(bottom_right), -100.0f, 100.0f);
	ImGui::SliderFloat3("Spawn Position", value_ptr(spawn_pos), -100.0f, 100.0f);
	ImGui::SliderFloat("Bounds Dampening", &bound_dampening, 0.0f, 1.0f);
	ImGui::SliderInt("Particle Count", &particle_count, 0, 100000);
	ImGui::SliderFloat3("Particle Scale", value_ptr(particle_scale), 0.0f, 1.0f);
	ImGui::Text("Water Properties");
	ImGui::SliderFloat("Smoothing Radius", &smoothing_radius, 0.0f, 100.0f);
	ImGui::SliderFloat("Mass", &mass, 0.0f, 100.0f);
	ImGui::SliderFloat("Target Density", &target_density, 0.0f, 100.0f);
	ImGui::SliderFloat("Pressure Multiplier", &pressure_multiplier, 0.0f, 100.0f);
	ImGui::SliderFloat("Viscosity", &viscosity, 0.0f, 100.0f);
	ImGui::SliderFloat("Gravity", &gravity, 0.0f, 100.0f);

	ImGui::End();
}

float water_sim::smoothing_kernel(float dist){
	// poly6
	const float Poly6_constant = (315.0f / (64.0f * PI * (float)pow(smoothing_radius, 9.0f)));
	if (dist > smoothing_radius) return  0.0f;
	return Poly6_constant * pow(smoothing_radius * smoothing_radius - dist * dist, 3.0f);
}

float water_sim::smoothing_kernel_derivative(float dist){
	// spiky
	const float Spiky_constant = (-45.0f / (PI * (float)pow(smoothing_radius, 6.0f)));
	if (dist > smoothing_radius) return 0.0f;
	return Spiky_constant * pow(smoothing_radius - dist, 2.0f);
}

void water_sim::calculate_pressure_density(int n){
	vec3 pos = particles[n].estimated_position;
	float density = 0.0f;
	for (int j = 0; j < (int)particles.size(); j++){
		vec3 diff = particles[j].position - pos;
		float dist = dot(diff, diff);
		dist = sqrt(dist);
		if (smoothing_radius * smoothing_radius > dist){
			density += mass * smoothing_kernel(dist);
		}
	}
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
				float w = -((float)pow(dist, 3.0f) / (2 * (float)pow(smoothing_radius, 3.0f))) + ((float)pow(dist, 2.0) / (float)pow(smoothing_radius, 2.0f)) + (smoothing_radius / (2 * dist)) - 1;
				viscosity_force += viscosity * (1.0f / particles[j].density) * (particles[j].velocity - particles[n].velocity) * dir * w;
			}
		}
	}
	return -pressure + vec3(0, -gravity, 0) + viscosity_force;		
}
