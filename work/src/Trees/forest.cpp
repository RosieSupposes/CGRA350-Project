#include "forest.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

forest::forest(int count){
    sharedMesh = cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//teapot.obj")).build();
	reload(count);
}

void forest::reload(int count){
	reset_trees(count);
}

void forest::reset_trees(int treeCount){
	trees.clear();
	for(int t = 0; t < treeCount; t++){
		float range = 38;
		vec3 randomPosition = vec3(
			-(range/2) + range*((float)std::rand())/RAND_MAX,//range between -10 to 10
			0,
			-(range/2) + range*((float)std::rand())/RAND_MAX //range between -10 to 10
		);
		trees.push_back(tree(translate(mat4(1), randomPosition)));
	}
}

void forest::simulate(){
	
}

void forest::draw(const mat4 &view, const mat4 &proj, GLuint shader) {
	for(tree t : trees){
		t.draw(view, proj, shader);
	}
}  
