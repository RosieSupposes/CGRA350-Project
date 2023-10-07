#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "tree.hpp"
#include "Other/material.hpp"
#include "Other/terrain.hpp"

class forest {
private:
    std::vector<tree> trees;
    
    void reset_trees(terrain terrain, int treeCount, int recursion_depth, string style);

public:
    forest();
    forest(terrain terrain, int count, int recursion_depth, string style);
    void reload(terrain terrain, int count, int recursion_depth, string style);
    void simulate();
	void draw(const glm::mat4 &view, const glm::mat4 &proj, material &trunk_material, material &tree_material);
};  
