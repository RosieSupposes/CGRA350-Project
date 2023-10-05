#include "forest.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

forest::forest(int count, int recursion_depth, string style){
	reload(count, recursion_depth, style);
}

void forest::reload(int count, int recurison_depth, string style){
	reset_trees(count, recurison_depth, style);
}

void forest::reset_trees(int treeCount, int recursion_depth, string style){
	trees.clear();
	for(int t = 0; t < treeCount; t++){
		float range = 38;
		vec3 randomPosition = vec3(
			-(range/2) + range*((float)std::rand())/RAND_MAX,//range between -10 to 10
			0,
			-(range/2) + range*((float)std::rand())/RAND_MAX //range between -10 to 10
		);
		trees.push_back(tree(translate(mat4(1), randomPosition), recursion_depth, style));
	}
}

void forest::simulate(){
	
}

void forest::draw(const mat4 &view, const mat4 &proj, GLuint shader) {
	for(tree t : trees){
		t.draw(view, proj, shader);
	}
}  
