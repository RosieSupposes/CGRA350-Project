#include "terrain.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "cgra/cgra_wavefront.hpp"

using namespace glm;


terrain::terrain(){
	
}

terrain::terrain(std::string file)
{
	cgra::mesh_builder mesh_builder = cgra::load_wavefront_data(file);
	m_model = basic_model(mesh_builder, scale(mat4(1), m_scale));
}
void terrain::draw(const glm::mat4 &view, const glm::mat4 &proj, material material)
{
	m_model.draw(view, proj, material);
}