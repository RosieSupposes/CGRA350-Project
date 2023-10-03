
// std
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

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


using namespace std;
using namespace cgra;
using namespace glm;


Application::Application(GLFWwindow *window) : m_window(window) {

	readSettings();
	
	loadShaders(styles[0]);
	
	fireflies = firefly_cluster(fireflyCount);
	trees = forest(treeCount);
	basic_water = basic_model(basic_water_shader, CGRA_SRCDIR + std::string("//res//assets//simple_water.obj"), vec3(0.0,0.9,0.9));
	terrain = basic_model(basic_water_shader, CGRA_SRCDIR + std::string("//res//assets//land.obj"), scale(mat4(1), vec3(8)));
}

void Application::loadShaders(const char * type){
	string style = std::string(type);
	std::cout << style << std::endl;
	//Check for contains long term, this is hack for now
	//if("PBR", "Sketched", "Pixel"){
	//	style = std::string("color");
	//}

	shader_builder water_sb, tree_sb, firefly_sb, simple_water_sb;
	string file_head = CGRA_SRCDIR + std::string("//res//shaders//") + style;
	
	water_shader = buildVertAndFragShader(file_head);
	tree_shader = buildVertAndFragShader(file_head);
	firefly_shader = buildVertAndFragShader(file_head);
	basic_water_shader = buildVertAndFragShader(file_head);
	std::cout << "Changed shader type to: " << style << std::endl;
}

GLuint Application::buildVertAndFragShader(string file_head){
	shader_builder builder;
	builder.set_shader(GL_VERTEX_SHADER, file_head + std::string("_vert.glsl"));
	builder.set_shader(GL_FRAGMENT_SHADER, file_head + std::string("_frag.glsl"));
	return builder.build();
}


void Application::render() {
	double start_time = glfwGetTime();
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
	
	//Simulate things
	simulate();

	// draw things
	renderTerrain(view, proj);
	renderFireflies(view, proj);
	renderWater(view, proj);
	renderTrees(view, proj);
	WrapUpFrame(start_time);
}

void Application::WrapUpFrame(double start_time){
	if(frames.size() > max_frames)
	{
		frames.clear();
	}
	double frameTime = 0;
	for(int i = 0; i < frames.size(); i++)
	{
		frameTime += frames[i];
	}
	double delay = 0;
	if(frameTime < frames.size()/max_frames){
		delay = 1/max_frames;
		std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)(1000*delay)));
	}
	double stop_time = glfwGetTime();
	frames.push_back(stop_time - start_time);
}

void Application::renderFireflies(const mat4 &view, const mat4 proj){
	fireflies.draw(view, proj, firefly_shader);
}


void Application::renderTrees(const mat4 &view, const mat4 proj){
	trees.draw(view, proj, tree_shader);
}

void Application::renderWater(const mat4 &view, const mat4 proj){
	if(water_sim_enabled)
	{
		water.draw(view, proj, water_shader);
	}
	else
	{
		basic_water.draw(view, proj);
	}
}

void Application::renderTerrain(const mat4 &view, const mat4 proj){
	terrain.draw(view, proj);
}

void Application::simulate(){
	simulateFireflies();
	simulateTrees();
	simulateWater();
}

void Application::simulateFireflies(){
	fireflies.simulate();
}

void Application::simulateTrees(){
	trees.simulate();
}

void Application::simulateWater(){
	if(water_sim_enabled)
	{
		water.simulate();
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

	ImGui::Text("Style");
	static int item_current = 0; // If the selection isn't within 0..count, Combo won't display a preview
    if(ImGui::Combo("Style", &item_current, styles, sizeof(styles)/sizeof(*styles))){
		loadShaders(styles[item_current]);
	}

	ImGui::Text("Water");
	ImGui::Checkbox("Water Sim Enabled", &water_sim_enabled);
	ImGui::Text("Fireflies");
	if (ImGui::InputInt("Fireflies", &fireflyCount)) {
		fireflies.reload(fireflyCount);
	}
	ImGui::Text("Trees");
	if (ImGui::InputInt("Trees", &treeCount)) {
		trees.reload(treeCount);
	}
	if (ImGui::InputInt("Recursion Depth", &recursion_depth)) {
		trees.reload(treeCount);
	}
	static int selected_style = 0;
	if(ImGui::Combo("Tree style", &selected_style, tree_styles, sizeof(tree_styles)/sizeof(*tree_styles))){
		loadShaders(styles[selected_style]);
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

void Application::readSettings(){
	// open settings file
	std::string filename = CGRA_SRCDIR + std::string("//default.settings");
	ifstream settingsFile(filename);
	if (!settingsFile.is_open()) {
		cerr << "Error: could not open " << filename << endl;
	}

	// good() means that failbit, badbit and eofbit are all not set
	while (settingsFile.good()) {

		// Pull out line from file
		string line;
		getline(settingsFile, line);
		istringstream settingsLine(line);

		// Pull out mode from line
		string mode;
		settingsLine >> mode;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		if (settingsLine.good()) {
			std::string outHolder;
			if (mode == "water_sim_enabled=") {
				settingsLine >> water_sim_enabled;
				std::cout << "Set water simulation default to " << (max_frames ? "On" : "Off") << std::endl;
			}
			else if (mode == "framerate_limit=") {
				settingsLine >> max_frames;
				std::cout << "Set frame rate limit to " << max_frames << std::endl;
			}
			else if (mode == "tree_recursion_depth=") {
				settingsLine >> recursion_depth;
				std::cout << "Set tree recursion depth to " << recursion_depth << std::endl;
			}
			else if (mode == "other_things") {
				std::string placeHolder;
				settingsLine >> placeHolder;
			}
		}
	}
}
