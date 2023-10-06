#include "basic_model.hpp"
#include <glm/gtc/matrix_transform.hpp>

//Placeholder
#include "cgra/cgra_wavefront.hpp"

using namespace glm;

basic_model::basic_model(){
	
}

basic_model::basic_model(material material, std::string file){
	m_material = material;
	m_mesh = cgra::load_wavefront_data(file).build();
}

basic_model::basic_model(material material, std::string file, vec3 colour){
	m_material = material;
	m_mesh = cgra::load_wavefront_data(file).build();
	m_color = colour;
}

basic_model::basic_model(material material, std::string file, mat4 transform){
	m_modelTransform = transform;
	m_material = material;
	m_mesh = cgra::load_wavefront_data(file).build();
}

basic_model::basic_model(material material, std::string file, mat4 transform, vec3 colour){
	m_modelTransform = transform;
	m_material = material;
	m_mesh = cgra::load_wavefront_data(file).build();
	m_color = colour;
}

void basic_model::draw(const mat4 &view, const mat4 &proj) {
	// set up the shader for every draw call
    mat4 modelview = view * m_modelTransform;
	m_material.load(modelview, proj);
	m_mesh.draw();
} 
