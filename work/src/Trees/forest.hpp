#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp"
#include "tree.hpp"

class forest {
private:
    cgra::gl_mesh sharedMesh;
    std::vector<tree> trees;
    
    void reset_trees(int treeCount);

public:
    forest(int count);
    void reload(int count);
    void simulate();
	void draw(const glm::mat4 &view, const glm::mat4 &proj, GLuint shader);
};  
