
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
	trees = forest(treeCount, recursion_depth, std::string(tree_styles[0]));
	basic_water = basic_model(basic_water_shader, CGRA_SRCDIR + std::string("//res//assets//simple_water.obj"), vec3(0.0,0.9,0.9));
	terrain = basic_model(basic_water_shader, CGRA_SRCDIR + std::string("//res//assets//land.obj"), scale(mat4(1), vec3(8)));

	//TODO uncomment when water_sim constructor matches this:
	//water = water_sim(water_shader, &boundDamping, &restDensity, &gasConstant, &viscosity, &particleMass, &smoothingRadius, &timeStep); 
}

void Application::loadShaders(const char * type){
	string style = std::string(type);

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

	//Update camera
	m_camera.updateProjection(width, height);
	m_camera.update();
	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);

	mat4 proj = m_camera.getProjection();
	mat4 view = m_camera.getView();
	

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
	int gap = 5;
	int mainWindowPos = gap;
	int mainWindowHeight = 160;
	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, mainWindowPos), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, mainWindowHeight), ImGuiSetCond_Once);
	ImGui::Begin("Options", 0);

	// display current camera parameters
	ImGui::Text("Application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("Pitch", &(m_camera.m_pitch), -pi<float>() / 2, pi<float>() / 2, "%.2f");
	ImGui::SliderFloat("Yaw", &(m_camera.m_yaw), -pi<float>(), pi<float>(), "%.2f");
	
	//OPTIONS
	ImGui::Text("OPTIONS");



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

	int fireflyWindowHeight = 100;
	int fireflyWindowPos = mainWindowPos + mainWindowHeight + gap;
	fireflies.renderGUI(fireflyWindowHeight, fireflyWindowPos);

	int shaderWindowHeight = 75;
	int shaderWindowPos = fireflyWindowPos + fireflyWindowHeight + gap;
	renderShaderGUI(shaderWindowHeight, shaderWindowPos);

	int treesWindowHeight = 125;
	int treesWindowPos = shaderWindowPos + shaderWindowHeight + gap;
	renderTreesGUI(treesWindowHeight, treesWindowPos);

	int waterWindowHeight = 230;
	int waterWindowPos = treesWindowPos + treesWindowHeight + gap;
	renderWaterGUI(waterWindowHeight, waterWindowPos);
}

void Application::renderShaderGUI(int height, int pos) {
	ImGui::SetNextWindowPos(ImVec2(5, pos), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, height), ImGuiSetCond_Once);
	ImGui::Begin("Style", 0);

	ImGui::Text("Style");
	static int selected_style = 0; // If the selection isn't within 0..count, Combo won't display a preview
	if (ImGui::Combo("Style", &selected_style, styles, sizeof(styles) / sizeof(*styles))) {
		loadShaders(styles[selected_style]);
	}

	ImGui::End();
}

void Application::renderTreesGUI(int height, int pos) {
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
		trees.reload(treeCount, recursion_depth, std::string(style));
	}

	ImGui::End();
}

void Application::renderWaterGUI(int height, int pos) {
	ImGui::SetNextWindowPos(ImVec2(5, pos), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, height), ImGuiSetCond_Once);
	ImGui::Begin("Water", 0);

	ImGui::Text("Water");
	ImGui::Checkbox("Water Sim Enabled", &water_sim_enabled);
	
	ImGui::InputFloat("Bound Damping", &boundDamping, 0.0f, 0.0f, 2); //boundDamping (-0.3)
	ImGui::InputFloat("Rest Density", &restDensity, 0.0f, 0.0f, 1); //restDensity (1.0)
	ImGui::InputFloat("Gas", &gasConstant, 0.0f, 0.0f, 1);  //gasConstant (2.0)
	ImGui::InputFloat("Viscosity", &viscosity, 0.0f, 0.0f, 4); //viscosity (-0.003)
	ImGui::InputFloat("Particle Mass", &particleMass, 0.0f, 0.0f, 1); //particleMass (1.0)
	ImGui::InputFloat("Smoothing Radius", &smoothingRadius, 0.0f, 0.0f, 1); //smoothingRadius (1.0)
	ImGui::InputFloat("Time Step", &timeStep, 0.0f, 0.0f, 4); //timeStep (0.001)

	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {
	if (m_leftMouseDown) {
		vec2 whsize = m_windowsize / 2.0f;

		// clamp the pitch to [-pi/2, pi/2]
		m_camera.m_pitch += float(acos(glm::clamp((m_mousePosition.y - whsize.y) / whsize.y, -1.0f, 1.0f))
			- acos(glm::clamp((float(ypos) - whsize.y) / whsize.y, -1.0f, 1.0f)));
		m_camera.m_pitch = float(glm::clamp(m_camera.m_pitch, -pi<float>() / 2, pi<float>() / 2));

		// wrap the yaw to [-pi, pi]
		m_camera.m_yaw += float(acos(glm::clamp((m_mousePosition.x - whsize.x) / whsize.x, -1.0f, 1.0f))
			- acos(glm::clamp((float(xpos) - whsize.x) / whsize.x, -1.0f, 1.0f)));
		if (m_camera.m_yaw > pi<float>()) m_camera.m_yaw -= float(2 * pi<float>());
		else if (m_camera.m_yaw < -pi<float>()) m_camera.m_yaw += float(2 * pi<float>());
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
	m_camera.move(vec3(0,0,2) * (float)(sign(yoffset) * pow(1.1f, -yoffset)));
}

enum keys{
	A = 65,
	D = 68,
	S = 83,
	W = 87,
	UP = 265,
	DOWN = 264,
	LEFT = 263,
	RIGHT = 262
};

void Application::keyCallback(int key, int scancode, int action, int mods) {
	std::cout << key << std::endl;
	float moveSpeed = 1;
	switch ((keys)key)
	{
	case A:
		std::cout << "A" << std::endl;
		adjustFocalPoint(vec3(1,0,0), false);
		break;
	case D:
		std::cout << "D" << std::endl;
		adjustFocalPoint(vec3(-1,0,0), false);
		break;
	case S:
		std::cout << "S" << std::endl;
		adjustFocalPoint(vec3(0,0,-1), false);
		break;
	case W:
		std::cout << "W" << std::endl;
		adjustFocalPoint(vec3(0,0,1), false);
		break;
	case UP:
		std::cout << "UP" << std::endl;
		adjustFocalPoint(vec3(0,0,1), false);
		break;
	case DOWN:
		std::cout << "DOWN" << std::endl;
		adjustFocalPoint(vec3(0,0,-1), false);
		break;
	case LEFT:
		std::cout << "LEFT" << std::endl;
		adjustFocalPoint(vec3(1,0,0), false);
		break;
	case RIGHT:
		std::cout << "RIGHT" << std::endl;
		adjustFocalPoint(vec3(-1,0,0), false);
		break;
	default:
		break;
	}
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}

void Application::adjustFocalPoint(vec3 amount, bool setAbsolute){
	if(setAbsolute){
		m_camera.move(amount);
	}else{
		m_camera.move(amount);
	}
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
