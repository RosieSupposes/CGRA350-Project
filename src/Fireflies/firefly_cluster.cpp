#include "firefly_cluster.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

firefly_cluster::firefly_cluster(int count){
    sharedMesh = cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//placeholderSphere.obj")).build();
	reload(count);
}

void firefly_cluster::reload(int count){
	reset_flies(count);
}

void firefly_cluster::reset_flies(int fireflyCount){
	fireflies.clear();
	for(int f = 0; f < fireflyCount; f++){
		float range = 38;
		vec3 randomPosition = vec3(
			-(range/2) + range*((float)std::rand())/RAND_MAX,//range between -10 to 10
			10 + 10*((float)std::rand())/RAND_MAX, //range between 10 to 20
			-(range/2) + range*((float)std::rand())/RAND_MAX //range between -10 to 10
		);
		fireflies.push_back(firefly(translate(mat4(1), randomPosition)));
	}
}

void firefly_cluster::simulate(){
	
}

void firefly_cluster::draw(const mat4 &view, const mat4 &proj, GLuint shader) {
	for(firefly ff : fireflies){
		ff.draw(view, proj, shader);
	}
}  
