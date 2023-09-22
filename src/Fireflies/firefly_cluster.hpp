#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "firefly.hpp"

#define e 2.71828

class firefly_cluster {
private:
    cgra::gl_mesh sharedMesh;
    std::vector<firefly> fireflies;

    float absorption = 10.0f;   //between 0.01 and 100
    float max_attraction = 1;   //apparently 1 is good
    float alpha;                //between 0 and 1
    float max_brightness = 1.0f; 
    float brightness_step = 0.1f;
    
    void reset_flies(int fireflyCount);
    float relative_brightness(firefly i, firefly j);
    float attraction(firefly i, firefly j);

public:
    firefly_cluster(int count);
    void reload(int count);
    void simulate();
	void draw(const glm::mat4 &view, const glm::mat4 &proj, GLuint shader);
};  
