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
    loadPoints(cgra::load_wavefront_data(file), mat4(1));
}

collider::collider(std::string file, mat4 transform)
{
    loadPoints(cgra::load_wavefront_data(file), transform);
}

collider::collider(cgra::mesh_builder mesh_builder)
{
    loadPoints(mesh_builder, mat4(1));
}

collider::collider(cgra::mesh_builder mesh_builder, mat4 transform)
{
    loadPoints(mesh_builder, transform);
}

void collider::loadPoints(cgra::mesh_builder mesh_builder, mat4 transform)
{
    for(int i = 0; i < mesh_builder.vertices.size(); i++)
    {
        vec3 pos = mesh_builder.vertices[i].pos;
        vec4 hCoords = vec4(pos.x, pos.y, pos.z, 1) * transform;
        vertices.push_back(vec3(hCoords.x, hCoords.y, hCoords.z));
    }
    for(int i = 0; i < mesh_builder.indices.size(); i++)
    {
        /*if(i%3==0) std::cout << "Triangle " << i/3 << ":" << std::endl;
        vec3 p = mesh_builder.vertices[mesh_builder.indices[i]].pos;
        std::cout << "Vertex " << i << ", point: (" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;*/
        indices.push_back(mesh_builder.indices[i]);
    }
}

Collision collider::checkCollision(Ray ray)
{
    /*std::cout << "Ray: " << std::endl;
    std::cout << "\t point: (" << ray.point.x << ", " << ray.point.y << ", " << ray.point.z << ")" << std::endl;
    std::cout << "\t direction: (" << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << ")" << std::endl;
    std::cout << "\t length" << ray.length << std::endl;*/
    Collision col;
    col.hit = false;
    for(int i = 0; i < indices.size(); i += 3)
    {
        //Points for the triangle
        vec3 a = vertices[indices[i]];
        vec3 b = vertices[indices[i+1]];
        vec3 c = vertices[indices[i+2]];
        
        //Calc normal
        vec3 bMinusA = b - a;
        vec3 cMinusA = c - a;
        
        vec3 n = normalize(cross(bMinusA, cMinusA));
        /*
        std::cout << "Triangle " << i/3 + 1 << ":" << std::endl;
        std::cout << "Vertex " << i << ", point: (" << a.x << ", " << a.y << ", " << a.z << ")" << std::endl;
        std::cout << "Vertex " << i+1 << ", point: (" << b.x << ", " << b.y << ", " << b.z << ")" << std::endl;
        std::cout << "Vertex " << i+2 << ", point: (" << c.x << ", " << c.y << ", " << c.z << ")" << std::endl;
        std::cout << "cMinusA is (" << cMinusA.x << ", " << cMinusA.y << ", " << cMinusA.z << ")" << std::endl;
        std::cout << "bMinusA is (" << bMinusA.x << ", " << bMinusA.y << ", " << bMinusA.z << ")" << std::endl;
        std::cout << "Normal is (" << n.x << ", " << n.y << ", " << n.z << ")" << std::endl;
        */

        //Check if parallel (n Dot d = 0)
        float tDenom = dot(n, ray.direction);
        if(tDenom == 0)
        {
            //Parallel, cannot collide with triangle
            continue;
        }
        //Calculate collision point with triangle

        float d = dot(n, a);
        float t = (d - dot(n, ray.point))/tDenom;
        if(t > ray.length)
        {
            //Cannot collide due to shorter distance.
            continue;
        }
        vec3 q = ray.point + t*ray.direction;

        //Do something for checking collision
        vec3 cMinusB = c - b;
        vec3 aMinusC = a - c;

        vec3 qMinusA = q - a;
        vec3 qMinusB = q - b;
        vec3 qMinusC = q - c;

        float check1 = dot(cross(bMinusA, qMinusA), n);
        float check2 = dot(cross(cMinusB, qMinusB), n);
        float check3 = dot(cross(aMinusC, qMinusC), n);

        if(check1 >= 0 && check2 >= 0 && check3 >= 0){
            //std::cout << "Collision!" << std::endl;
            //collided, return valid collision
            
            //std::cout << "Triangle " << i/3 + 1 << ":" << std::endl;
            ////std::cout << "Vertex " << i << ", point: (" << a.x << ", " << a.y << ", " << a.z << ")" << std::endl;
            //std::cout << "Vertex " << i+1 << ", point: (" << b.x << ", " << b.y << ", " << b.z << ")" << std::endl;
            //std::cout << "Vertex " << i+2 << ", point: (" << c.x << ", " << c.y << ", " << c.z << ")" << std::endl;
            //std::cout << "Normal is (" << n.x << ", " << n.y << ", " << n.z << ")" << std::endl;
            col.hit = true;
            col.point = q;
            col.normal = n;
            return col;
        }
    }
    return col;
}

bool collider::doesCollide(Ray ray)
{
    return checkCollision(ray).hit;
}