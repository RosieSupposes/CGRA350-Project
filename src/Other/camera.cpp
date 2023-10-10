#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;

camera::camera(){
	m_view = rotate(mat4(1), m_pitch, vec3(1, 0, 0))
		* rotate(mat4(1), m_yaw,   vec3(0, 1, 0))
		* translate(mat4(1), m_position);
}

camera::camera(vec3 position){
	m_view = rotate(mat4(1), m_pitch, vec3(1, 0, 0))
		* rotate(mat4(1), m_yaw,   vec3(0, 1, 0))
		* translate(mat4(1), m_position);
	m_position = position;
}

void camera::move(vec3 displacement){
	vec4 offset = 0.2f*vec4(displacement.x, displacement.y, displacement.z, 1) * m_view;
	m_velocity += vec3(offset.x, offset.y, offset.z);
	if(length(m_velocity) > m_maxVelocity){
		m_velocity = m_maxVelocity * normalize(m_velocity);
	}
}

void camera::update(){
	m_position += m_velocity;
	setInBounds();
	m_view = rotate(mat4(1), m_pitch, vec3(1, 0, 0))
		* rotate(mat4(1), m_yaw,   vec3(0, 1, 0))
		* translate(mat4(1), m_position);
}

void camera::setInBounds(){
	m_position = vec3(min(m_position.x, maxBounds.x), min(m_position.y, maxBounds.y), min(m_position.z, maxBounds.z));
	m_position = vec3(max(m_position.x, minBounds.x), max(m_position.y, minBounds.y), max(m_position.z, minBounds.z));
}

void camera::reduceVelocity(){
	m_velocity = 0.8f*m_velocity;
	if(length(m_velocity) < m_velocityThreshold){
		m_velocity = vec3(0);
	}
}

void camera::updateProjection(int frameWidth, int frameHeight){
	if(frameHeight != 0){
		m_proj = perspective(1.f, float(frameWidth) / frameHeight, 0.1f, 1000.f);
	}
	
}

mat4 camera::getProjection(){
	return m_proj;
}

mat4 camera::getView(){
	return m_view;
}
