#include "water_sim.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "cgra/cgra_wavefront.hpp"
#include "cgra/cgra_geometry.hpp"

using namespace std;
using namespace glm;
using namespace cgra;

water_sim::water_sim(){
	for (int x = 0; x < 5; x++){
		for (int y = 0; y < 5; y++){
			for (int z = 0; z < 5; z++){
				create_particle(vec3(-10 + x * 4, -10 + y * 4, -10 + z * 4));
			}
		}
	}
}

void water_sim::simulate(){
	for (int i = 0; i < (int)particles.size(); i++){
		particles[i].simulate(timestep);
		manageBoundry(particles[i].velocity, particles[i].position);
	}
}

void water_sim::reload(){}

void water_sim::draw(const mat4 &view, const mat4 &proj, GLuint shader) {
	for (int i = 0; i < (int)particles.size(); i++){
		particles[i].draw(view, proj, shader);
	}


} 

void water_sim::create_particle(glm::vec3 position){
	Particle p;
	p.position = position;
	p.neighbours.push_back(&p);
	particles.push_back(p);
}

void water_sim::manageBoundry(vec3 &vel, vec3 &position){
    if (position.x - 1 < bottomLeft.x){
        vel.x *= boundDamping;
        position.x = bottomLeft.x + 1;
    }
	if (position.x + 1 > topRight.x){
		vel.x *= boundDamping;
		position.x = topRight.x - 1;
	}
	if (position.y - 1 < bottomLeft.y){
		vel.y *= boundDamping;
		position.y = bottomLeft.y + 1;
	}
	if (position.y + 1 > topRight.y){
		vel.y *= boundDamping;
		position.y = topRight.y - 1;
	}
	if (position.z - 1 < bottomLeft.z){
		vel.z *= boundDamping;
		position.z = bottomLeft.z + 1;
	}
	if (position.z + 1 > topRight.z){
		vel.z *= boundDamping;
		position.z = topRight.z - 1;
	}
}
