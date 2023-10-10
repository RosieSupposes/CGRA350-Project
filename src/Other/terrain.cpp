#include "terrain.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "cgra/cgra_wavefront.hpp"

using namespace glm;


terrain::terrain()
{
	
}

terrain::terrain(std::string file)
{
	cgra::mesh_builder mesh_builder = cgra::load_wavefront_data(file);
	m_model = basic_model(mesh_builder);
	m_collider = collider(mesh_builder);
}

terrain::terrain(std::string file, mat4 transform)
{
	cgra::mesh_builder mesh_builder = cgra::load_wavefront_data(file);
	m_model = basic_model(mesh_builder, transform);
	m_collider = collider(mesh_builder, transform);
}

bool terrain::doesCollide(Ray ray)
{
	return m_collider.doesCollide(ray);
}

Collision terrain::checkCollision(Ray ray)
{
	return m_collider.checkCollision(ray);
}

void terrain::draw(const glm::mat4 &view, const glm::mat4 &proj, material material)
{
	m_model.draw(view, proj, material);
}