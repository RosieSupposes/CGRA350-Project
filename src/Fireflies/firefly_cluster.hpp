#pragma once

#include <random>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "firefly.hpp"
#include "Other/material.hpp"

#define Math_E 2.71828

static std::mt19937 gen(std::random_device{}());

class firefly_cluster {
private:
	cgra::gl_mesh sharedMesh;
	std::vector<firefly> fireflies;

	//std::random_device rd; // obtain a random number from hardware
	//std::mt19937 gen;
	std::uniform_real_distribution<> random_small_vector;

	int fireflyCount = 100;

	float absorption = 10.0f;   //between 0.01 and 100
	float max_attraction = 1;   //apparently 1 is good
	float alpha = 0.5f;                //between 0 and 1
	float max_brightness = 1.0f;
	float brightness_step = 0.001f;

	float neighbourhood_size = 12.0f;

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
	void limit_velocity(firefly& f);
	void get_closest_fireflies(firefly& f);

	float get_random_num(float lower, float upper);

public:
	firefly_cluster(int count);
	void reload(int count);
	void simulate();
	void draw(const glm::mat4& view, const glm::mat4& proj, material &material);
	void renderGUI(int height, int pos);

};
