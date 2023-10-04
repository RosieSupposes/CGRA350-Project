#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;

camera::camera(){
	m_view = rotate(mat4(1), m_pitch, vec3(1, 0, 0))
		* rotate(mat4(1), m_yaw,   vec3(0, 1, 0))
		* translate(mat4(1), m_position);
}
camera::camera(glm::vec3 position){
	m_view = rotate(mat4(1), m_pitch, vec3(1, 0, 0))
		* rotate(mat4(1), m_yaw,   vec3(0, 1, 0))
		* translate(mat4(1), m_position);
	m_position = position;
}
void camera::move(glm::vec3 displacement){
	vec4 offset = vec4(displacement.x, displacement.y, displacement.z, 1) * m_view;
	m_position += vec3(offset.x, offset.y, offset.z);
}
void camera::update(){
	m_view = rotate(mat4(1), m_pitch, vec3(1, 0, 0))
		* rotate(mat4(1), m_yaw,   vec3(0, 1, 0))
		* translate(mat4(1), m_position);
}

void camera::updateProjection(int frameWidth, int frameHeight){
	m_proj = perspective(1.f, float(frameWidth) / frameHeight, 0.1f, 1000.f);
}
mat4 camera::getProjection(){
	return m_proj;
}
mat4 camera::getView(){
	return m_view;
}
