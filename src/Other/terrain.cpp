#include "terrain.hpp"
#include <glm/gtc/matrix_transform.hpp>

//Placeholder
#include "cgra/cgra_wavefront.hpp"

using namespace glm;

terrain::terrain(){
	
}
terrain::terrain(GLuint shader, std::string file){
	m_model = basic_model(shader, file);
	
}
terrain::terrain(GLuint shader, std::string file, glm::mat4 transform){
	m_model = basic_model(shader, file, transform);
}
void terrain::draw(const glm::mat4 &view, const glm::mat4 &proj){
	m_model.draw(view, proj);
}
