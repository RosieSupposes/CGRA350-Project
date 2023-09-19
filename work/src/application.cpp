
// std
#include <iostream>
#include <string>
#include <chrono>

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

// project
#include "application.hpp"

//Are these necessary?
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_image.hpp"
#include "cgra/cgra_shader.hpp"
#include "cgra/cgra_wavefront.hpp"


#include "firefly.hpp"
#include "simple_water.hpp"
#include "water_sim.hpp"
#include "tree.hpp"


using namespace std;
using namespace cgra;
using namespace glm;


Application::Application(GLFWwindow *window) : m_window(window) {
	
	shader_builder water_sb, tree_sb, firefly_sb, simple_water_sb;
	
    water_sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_vert.glsl"));
	water_sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_frag.glsl"));
	water_shader = water_sb.build();
	
	tree_sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_vert.glsl"));
	tree_sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_frag.glsl"));
	tree_shader = tree_sb.build();
	
	firefly_sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_vert.glsl"));
	firefly_sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_frag.glsl"));
	firefly_shader = firefly_sb.build();
	
	simple_water_sb.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_vert.glsl"));
	simple_water_sb.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//color_frag.glsl"));
	basic_water_shader = simple_water_sb.build();
	
	water = water_sim(water_shader);

	for(int t = 0; t < treeCount; t++){
		float range = 38;
		vec3 randomPosition = vec3(
			-(range/2) + range*((float)std::rand())/RAND_MAX,//range between -10 to 10
			0,
			-(range/2) + range*((float)std::rand())/RAND_MAX //range between -10 to 10
		);
		trees.push_back(tree(tree_shader, translate(mat4(1), randomPosition)));
	}
	for(int f = 0; f < fireflyCount; f++){
		float range = 38;
		vec3 randomPosition = vec3(
			-(range/2) + range*((float)std::rand())/RAND_MAX,//range between -10 to 10
			10 + 10*((float)std::rand())/RAND_MAX, //range between 10 to 20
			-(range/2) + range*((float)std::rand())/RAND_MAX //range between -10 to 10
		);
		fireflies.push_back(firefly(firefly_shader, translate(mat4(1), randomPosition)));
	}
	basic_water = simple_water(basic_water_shader);
}


void Application::render() {
	
	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height); 

	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);

	// projection matrix
	mat4 proj = perspective(1.f, float(width) / height, 0.1f, 1000.f);

	// view matrix
	mat4 view = translate(mat4(1), vec3(0, 0, -m_distance))
		* rotate(mat4(1), m_pitch, vec3(1, 0, 0))
		* rotate(mat4(1), m_yaw,   vec3(0, 1, 0));


	// helpful draw options
	if (m_show_grid) drawGrid(view, proj);
	if (m_show_axis) drawAxis(view, proj);
	glPolygonMode(GL_FRONT_AND_BACK, (m_showWireframe) ? GL_LINE : GL_FILL);


	// draw things
	renderFireflies(view, proj);
	renderWater(view, proj);
	renderTrees(view, proj);
}

void Application::renderFireflies(const glm::mat4 &view, const glm::mat4 proj){
	for(firefly ff : fireflies){
		ff.draw(view, proj);
	}
}


void Application::renderTrees(const glm::mat4 &view, const glm::mat4 proj){
	for(tree t : trees){
		t.draw(view, proj);
	}
}



void Application::renderWater(const glm::mat4 &view, const glm::mat4 proj){
	if(water_sim_enabled)
	{
		water.draw(view, proj);
	}
	else
	{
		basic_water.draw(view, proj);
	}
}


void Application::renderGUI() {

	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Once);
	ImGui::Begin("Options", 0);

	// display current camera parameters
	ImGui::Text("Application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("Pitch", &m_pitch, -pi<float>() / 2, pi<float>() / 2, "%.2f");
	ImGui::SliderFloat("Yaw", &m_yaw, -pi<float>(), pi<float>(), "%.2f");
	ImGui::SliderFloat("Distance", &m_distance, 0, 100, "%.2f", 2.0f);
	
	//OPTIONS
	ImGui::Text("OPTIONS");
	ImGui::Checkbox("Water Sim Enabled", &water_sim_enabled);
	if (ImGui::InputInt("Fireflies", &fireflyCount)) {
		fireflies.clear();
		for(int f = 0; f < fireflyCount; f++){
			float range = 38;
			vec3 randomPosition = vec3(
				-(range/2) + range*((float)std::rand())/RAND_MAX,//range between -10 to 10
				10 + 10*((float)std::rand())/RAND_MAX, //range between 10 to 20
				-(range/2) + range*((float)std::rand())/RAND_MAX //range between -10 to 10
			);
			fireflies.push_back(firefly(firefly_shader, translate(mat4(1), randomPosition)));
		}
	}
	if (ImGui::InputInt("Trees", &treeCount)) {
		trees.clear();
		for(int t = 0; t < treeCount; t++){
			float range = 38;
			vec3 randomPosition = vec3(
				-(range/2) + range*((float)std::rand())/RAND_MAX,//range between -10 to 10
				0,
				-(range/2) + range*((float)std::rand())/RAND_MAX //range between -10 to 10
			);
			trees.push_back(tree(tree_shader, translate(mat4(1), randomPosition)));
		}
	}
	ImGui::Separator();
	// helpful drawing options
	ImGui::Checkbox("Show axis", &m_show_axis);
	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &m_show_grid);
	ImGui::Checkbox("Wireframe", &m_showWireframe);
	ImGui::SameLine();
	if (ImGui::Button("Screenshot")) rgba_image::screenshot(true);

	
	


	// finish creating window
	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {
	if (m_leftMouseDown) {
		vec2 whsize = m_windowsize / 2.0f;

		// clamp the pitch to [-pi/2, pi/2]
		m_pitch += float(acos(glm::clamp((m_mousePosition.y - whsize.y) / whsize.y, -1.0f, 1.0f))
			- acos(glm::clamp((float(ypos) - whsize.y) / whsize.y, -1.0f, 1.0f)));
		m_pitch = float(glm::clamp(m_pitch, -pi<float>() / 2, pi<float>() / 2));

		// wrap the yaw to [-pi, pi]
		m_yaw += float(acos(glm::clamp((m_mousePosition.x - whsize.x) / whsize.x, -1.0f, 1.0f))
			- acos(glm::clamp((float(xpos) - whsize.x) / whsize.x, -1.0f, 1.0f)));
		if (m_yaw > pi<float>()) m_yaw -= float(2 * pi<float>());
		else if (m_yaw < -pi<float>()) m_yaw += float(2 * pi<float>());
	}

	// updated mouse position
	m_mousePosition = vec2(xpos, ypos);
}


void Application::mouseButtonCallback(int button, int action, int mods) {
	(void)mods; // currently un-used

	// capture is left-mouse down
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		m_leftMouseDown = (action == GLFW_PRESS); // only other option is GLFW_RELEASE
}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset; // currently un-used
	m_distance *= pow(1.1f, -yoffset);
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used
}
