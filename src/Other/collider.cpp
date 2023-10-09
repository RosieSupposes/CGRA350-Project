#include "collider.hpp"
#include <glm/gtc/matrix_transform.hpp>

//Placeholder
#include "cgra/cgra_wavefront.hpp"

using namespace glm;


collider::collider()
{

}

collider::collider(std::string file)
{
    loadPoints(cgra::load_wavefront_data(file));
}

collider::collider(cgra::mesh_builder mesh_builder)
{
    loadPoints(mesh_builder);
}

void collider::loadPoints(cgra::mesh_builder mesh_builder)
{
    for(int i = 0; i < mesh_builder.vertices.size(); i++)
    {
        vertices.push_back(mesh_builder.vertices[i].pos);
    }
    for(int i = 0; i < mesh_builder.indices.size(); i++)
    {
        indices.push_back(mesh_builder.indices[i]);
    }
}


bool collider::checkIntersection(Ray ray)
{
    return true;
}

vec3 collider::intersectionPoint(Ray ray)
{
    return vec3(0);
}