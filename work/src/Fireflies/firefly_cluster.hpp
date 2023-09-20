#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "firefly.hpp"

class firefly_cluster {
private:
    cgra::gl_mesh sharedMesh;
    std::vector<firefly> fireflies;
    
    void reset_flies(int fireflyCount);

public:
    firefly_cluster(int count);
    void reload(int count);
    void simulate();
	void draw(const glm::mat4 &view, const glm::mat4 &proj, GLuint shader);
};  
