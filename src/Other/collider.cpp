#include "collider.hpp"
#include "cgra/cgra_mesh.hpp"
#include <glm/gtc/matrix_transform.hpp>

//Placeholder
#include "cgra/cgra_wavefront.hpp"

using namespace glm;

collider::collider(){
}

//HOW AM I GOING TO DO COLLISIONS?
collider::collider(std::string file){
	std::vector<vec3> temp_points;
	cgra::mesh_builder m = cgra::load_wavefront_data(file);
	for(int i = 0; i < m.vertices.size(); i++){
		vec3 p = m.vertices[i].pos;
		temp_points.push_back(p);
	}
	for(int i = 0; i < m.indices.size(); i += 3){
		vec3 p0 = m.vertices[i].pos;
		vec3 p1 = m.vertices[i+1].pos;
		vec3 p2 = m.vertices[i+2].pos;
	}
}

glm::vec3 collider::nearestPointOnCollider(glm::vec3 point){
	return vec3(0);
}

glm::vec3 kd_node::findNearestPoint(glm::vec3 point){
	return vec3(0);
}
