#include "terrain.hpp"
#include <glm/gtc/matrix_transform.hpp>

//Placeholder
#include "cgra/cgra_wavefront.hpp"

using namespace glm;


terrain::terrain(){
	
}

terrain::terrain(std::string file){
	m_model = basic_model(file, scale(mat4(1), m_scale));
}
void terrain::draw(const glm::mat4 &view, const glm::mat4 &proj, material material){
	m_model.draw(view, proj, material);
}