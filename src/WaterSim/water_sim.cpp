#include "water_sim.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "cgra/cgra_wavefront.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include <cstdlib> 

using namespace std;
using namespace glm;
using namespace cgra;

water_sim::water_sim(){
	for (int x = 0; x < 10; x++){
		for (int y = 0; y < 5; y++){
			for (int z = 0; z < 10; z++){
				vec3 random = vec3((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX);
				create_particle(vec3(-10 + x * 2, 40 + y * 2, -10 + z * 2) + random);
			}
		}
	}
	for (int i = 0; i < (int)particles.size(); i++){
		Particle &p = particles[i];
		// Calculate Density
		calculate_density(p);

		// Calculate Pressure
		calculate_pressure(p);
	}
}

water_sim::water_sim(bool* enabled) {
	water_sim::enabled = enabled;
	for (int x = 0; x < 10; x++){
		for (int y = 0; y < 5; y++){
			for (int z = 0; z < 10; z++){
				vec3 random = vec3((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX);
				create_particle(vec3(-10 + x * 2, 40 + y * 2, -10 + z * 2) + random);
			}
		}
	}
	for (int i = 0; i < (int)particles.size(); i++){
		Particle &p = particles[i];
		// Calculate Density
		calculate_density(p);

		// Calculate Pressure
		calculate_pressure(p);
	}
}

void water_sim::simulate(){
	if (count >= 5){
		count = 0;
		create_particle(vec3(0, (topRight.y - bottomLeft.y) / 2.0f, 0));
	}
	for (int i = 0; i < (int)particles.size(); i++){
		Particle &p = particles[i];
		// Calculate Density
		calculate_density(p);

		// Calculate Pressure
		calculate_pressure(p);
		// Calculate Forces
		p.currentForce = calculate_forces(p);
	}

	// Update Positions
	for (int i = 0; i < (int)particles.size(); i++){
		Particle &p = particles[i];
		p.velocity += (p.currentForce / p.mass) * timestep;
		p.position += p.velocity * timestep;
		manage_boundry(particles[i].velocity, particles[i].position);
	}
}

void water_sim::reload(){}

void water_sim::draw(const mat4 &view, const mat4 &proj, material &material) {
	for (int i = 0; i < (int)particles.size(); i++){
		particles[i].draw(view, proj, material);
	}

	// draw bounding box
	//draw_boundary(view, proj, shader);
}

void water_sim::renderGUI(int height, int pos){
	ImGui::SetNextWindowPos(ImVec2(5, pos), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, height), ImGuiSetCond_Once);
	ImGui::Begin("Water", 0);

	ImGui::Text("Water");
	ImGui::Checkbox("Water Sim Enabled", enabled);
	
	ImGui::InputFloat("Bound Damping", &boundDamping, 0.0f, 0.0f, 2); //boundDamping (-0.3)
	ImGui::InputFloat("Rest Density", &restDensity, 0.0f, 0.0f, 1); //restDensity (1.0)
	ImGui::InputFloat("Gas", &gasConstant, 0.0f, 0.0f, 1);  //gasConstant (2.0)
	ImGui::InputFloat("Viscosity", &viscosity, 0.0f, 0.0f, 4); //viscosity (-0.003)
	ImGui::InputFloat("Particle Mass", &particleMass, 0.0f, 0.0f, 1); //particleMass (1.0)
	ImGui::InputFloat("Smoothing Radius", &smoothingRadius, 0.0f, 0.0f, 1); //smoothingRadius (1.0)
	ImGui::InputFloat("Time Step", &timestep, 0.0f, 0.0f, 4); //timeStep (0.001)

	ImGui::End();
}

void water_sim::draw_boundary(const mat4 &view, const mat4 &proj, material &material){
	// draw bounding box
	mat4 modelview = view * mat4(1);
	material.load(modelview, proj);
	glLineWidth(2);
	glBegin(GL_LINES);
	// bottom
	glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
	glVertex3f(topRight.x, bottomLeft.y, bottomLeft.z);
	glVertex3f(topRight.x, bottomLeft.y, bottomLeft.z);
	glVertex3f(topRight.x, bottomLeft.y, topRight.z);
	glVertex3f(topRight.x, bottomLeft.y, topRight.z);
	glVertex3f(bottomLeft.x, bottomLeft.y, topRight.z);
	glVertex3f(bottomLeft.x, bottomLeft.y, topRight.z);
	glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
	// top
	glVertex3f(bottomLeft.x, topRight.y, bottomLeft.z);
	glVertex3f(topRight.x, topRight.y, bottomLeft.z);
	glVertex3f(topRight.x, topRight.y, bottomLeft.z);
	glVertex3f(topRight.x, topRight.y, topRight.z);
	glVertex3f(topRight.x, topRight.y, topRight.z);
	glVertex3f(bottomLeft.x, topRight.y, topRight.z);
	glVertex3f(bottomLeft.x, topRight.y, topRight.z);
	glVertex3f(bottomLeft.x, topRight.y, bottomLeft.z);
	// sides
	glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
	glVertex3f(bottomLeft.x, topRight.y, bottomLeft.z);
	glVertex3f(topRight.x, bottomLeft.y, bottomLeft.z);
	glVertex3f(topRight.x, topRight.y, bottomLeft.z);
	glVertex3f(topRight.x, bottomLeft.y, topRight.z);
	glVertex3f(topRight.x, topRight.y, topRight.z);
	glVertex3f(bottomLeft.x, bottomLeft.y, topRight.z);
	glVertex3f(bottomLeft.x, topRight.y, topRight.z);
	glEnd();
}

Particle* water_sim::create_particle(glm::vec3 position){
	Particle p;
	p.position = position;
	p.mass = particleMass;
	particles.push_back(p);
	return &p;
}

void water_sim::manage_boundry(vec3 &vel, vec3 &position){
    if (position.x - 0.5 < bottomLeft.x){
        vel.x *= boundDamping;
        position.x = bottomLeft.x + 2;
    }
	if (position.x + 0.5 > topRight.x){
		vel.x *= boundDamping;
		position.x = topRight.x - 2;
	}
	if (position.y - 0.5 < bottomLeft.y){
		vel.y *= boundDamping;
		position.y = bottomLeft.y + 2;
	}
	if (position.y + 0.5 > topRight.y){
		vel.y *= boundDamping;
		position.y = topRight.y - 2;
	}
	if (position.z - 0.5 < bottomLeft.z){
		vel.z *= boundDamping;
		position.z = bottomLeft.z + 2;
	}
	if (position.z + 0.5 > topRight.z){
		vel.z *= boundDamping;
		position.z = topRight.z - 2;
	}
}

void water_sim::calculate_density(Particle &particle){
	float density = 0.0f;
	for (int i = 0; i < (int)particles.size(); i++){
		Particle &n = particles[i];
		float r = distance(particle.position, n.position);
		density += n.mass * poly6_kernel(r, smoothingRadius);
	}
	particle.density = density;
}

void water_sim::calculate_pressure(Particle &particle){
	particle.pressure = gasConstant * (particle.density - restDensity);
}

vec3 water_sim::calculate_forces(Particle &particle){
	vec3 pressureForce = vec3(0, 0, 0);
	vec3 viscosityForce = vec3(0, 0, 0);

	for (int i = 0; i < (float)particles.size(); i++){
		Particle &n = particles[i];
		if (&particle == &n) continue;
		float r = distance(particle.position, n.position);
		if (r > smoothingRadius * 2) continue;

		// Pressure Force
		vec3 dir = normalize(particle.position - n.position);
		pressureForce += spiky_gradient(r, dir) * (float)(particle.pressure / pow(particle.density, 2) + n.pressure / pow(n.density, 2));

		// Viscosity Force
		viscosityForce += viscosity * (n.velocity - particle.velocity) / n.density * spiky_kernel2(r, smoothingRadius);
	}
	return pressureForce + viscosityForce + vec3(0, -9.8f, 0);
}

float water_sim::poly6_kernel(float r, float h){
	if (r >= 0 && r <= h) {
        float coeff = 315.0f / (64.0f * 3.141592f * h*h*h*h*h*h*h*h*h);
        float q = (h*h - r*r);
        return coeff * q * q * q;
    } else {
        return 0.00000001f;
    }
}

float water_sim::spiky_kernel(float r, float h){
	float value = -45.0f / (3.141592f * (float)pow(h, 4)) * (float)pow(1-r/h, 2);
	return value;
}

float water_sim::spiky_kernel2(float r, float h){
	float value = -90.0f / (3.141592f * (float)pow(h, 5)) * (1-r/h);
	return value;
}

vec3 water_sim::spiky_gradient(float r, vec3 dir){
	return spiky_kernel(r, smoothingRadius) * dir;
}