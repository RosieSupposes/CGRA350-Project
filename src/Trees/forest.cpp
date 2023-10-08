#include "forest.hpp"


#include <glm/gtc/matrix_transform.hpp>
#include "opengl.hpp"

using namespace glm;

forest::forest(){
	
}

void forest::renderGUI(terrain terrain, int height, int pos) {
    ImGui::SetNextWindowPos(ImVec2(5, pos), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, height), ImGuiSetCond_Once);
    ImGui::Begin("Trees", 0);

    ImGui::Text("Trees");
    static int selected_tree_style = 0;
    bool tree_style_changed = ImGui::Combo("Tree style", &selected_tree_style, tree_styles, sizeof(tree_styles) / sizeof(*tree_styles));
    bool tree_count_changed = ImGui::InputInt("Trees", &treeCount);
    bool tree_depth_changed = ImGui::InputInt("Recursion Depth", &recursion_depth);
    if (tree_style_changed || tree_count_changed || tree_depth_changed) {
        string style = std::string(tree_styles[selected_tree_style]);
        reload(terrain, treeCount, recursion_depth, std::string(style));
    }

    ImGui::End();
}

forest::forest(terrain terrain){
	reload(terrain, treeCount, recursion_depth, std::string(tree_styles[0]));
}

void forest::reload(terrain terrain, int count, int recurison_depth, string style){
	reset_trees(terrain, count, recurison_depth, style);
}

void forest::reset_trees(terrain terrain, int treeCount, int recursion_depth, string style){
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

void forest::draw(const mat4 &view, const mat4 &proj, material &trunk_material, material &leaf_material) {
	for(tree t : trees){
		t.draw(view, proj, trunk_material, leaf_material);
	}
}  
