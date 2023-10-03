#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "firefly.hpp"

#define Math_E 2.71828

class firefly_cluster {
private:
    cgra::gl_mesh sharedMesh;
    std::vector<firefly> fireflies;

    float absorption = 10.0f;   //between 0.01 and 100
    float max_attraction = 1;   //apparently 1 is good
    float alpha = 0.5f;                //between 0 and 1
    float max_brightness = 1.0f; 
    float brightness_step = 0.001f;

    float dist = 3.0f;
    
    void reset_flies(int fireflyCount);
    float relative_brightness(firefly i, firefly j);
    float attraction(firefly i, firefly j);

    glm::vec3 min_bounds;
    glm::vec3 max_bounds;

    float speed_limit = 0.01f;

    glm::vec3 towards_brightest(firefly f);
    glm::vec3 away_from_each_other(firefly f);
    glm::vec3 within_bounds(firefly f);
    glm::vec3 fly_to_centre(firefly f);
    glm::vec3 match_velocity(firefly f);
    void limit_velocity(firefly &f);
    std::vector<firefly> get_closest_fireflies(firefly f);

    float get_random_num(float lower, float upper);

public:
    firefly_cluster(int count);
    void reload(int count);
    void simulate();
	void draw(const glm::mat4 &view, const glm::mat4 &proj, GLuint shader);
};  
