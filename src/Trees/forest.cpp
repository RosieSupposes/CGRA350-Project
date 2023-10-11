#include "forest.hpp"


#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "opengl.hpp"
#include "Other/collider.hpp"
#include "Other/RNG.hpp"

using namespace glm;

forest::forest(){

}

forest::forest(terrain terrain, mat4 transform, int treeCount, int recursion_depth, string style)
{
	m_transform = transform;
	reload(terrain, treeCount, recursion_depth, style);
}

void forest::reload(terrain terrain, int count, int recurison_depth, string style)
{
	reset_trees(terrain, count, recurison_depth, style);
}

void forest::reset_trees(terrain terrain, int treeCount, int recursion_depth, string style){
	trees.clear();
	float scale = 30;
	Ray ray{vec3(0), vec3(0,-1,0), 20};
	for(int t = 0; t < treeCount; t++)
	{
		float x = RNG::getRandomFloat(-scale, scale);
		float z = RNG::getRandomFloat(-scale, scale);
		vec4 pp = m_transform * vec4(x,ray.length+1,z,1);
		ray.point = vec3(pp.x,pp.y,pp.z);	
		Collision col = terrain.checkCollision(ray);
		if(col.hit){
			std::cout << "Placed tree " << t << " at point: (" << col.point.x << ", " << col.point.y 
			<< ", " << col.point.z << ")" << std::endl;
			trees.push_back(tree(translate(mat4(1), col.point+vec3(0,-0.2, 0)), recursion_depth, style));
		}
		else
		{
			std::cerr << "Tree missed terrain! point: (" << ray.point.x << ", " << ray.point.y 
			<< ", " << ray.point.z << ")" << std::endl;
			t--;
		}
	}
}

void forest::simulate(){
	
}

void forest::draw(const mat4 &view, const mat4 &proj, material &trunk_material, material &leaf_material) {
	for(tree t : trees){
		t.draw(view, proj, trunk_material, leaf_material);
	}
}  
