
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
	m_terrain = terrain(CGRA_SRCDIR + std::string("//res//assets//land.obj"));

	trees = forest(m_terrain);
	basic_water = basic_model(CGRA_SRCDIR + std::string("//res//assets//simple_water.obj"), vec3(0.0,0.9,0.9));

	//TODO uncomment when water_sim constructor matches this:
	m_water = water_sim(&boundDamping, &restDensity, &gasConstant, &viscosity, &particleMass, &smoothingRadius, &timeStep); 

	m_camera = camera();
	m_controller = keyboard_controller();
	m_controller.m_camera = &m_camera;
}

void Application::loadShaders(const char * type){
	string style = std::string(type);

	string file_head = CGRA_SRCDIR + std::string("//res//shaders//") + style;
	
	GLuint shader = buildVertAndFragShader(file_head);
	if(style == "PBR")
	{
		//Load textures for each material
			//fireflies(???)
			//treetrunk
			//leaves
			//terrain
			//water
			//advanced water
		GLuint firefly_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//fireflyPBR.png")).uploadTexture();
		GLuint trunk_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//trunkPBR.png")).uploadTexture();
		GLuint leaf_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//leafPBR.png")).uploadTexture();
		GLuint terrain_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//terrainPBR.png")).uploadTexture();
		GLuint basic_water_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//waterPBR.png")).uploadTexture();
		GLuint water_sim_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//waterSimPBR.png")).uploadTexture();
		m_firefly_material = material(shader, firefly_texture, vec3(1,1,0));
		m_trunk_material = material(shader, trunk_texture, vec3(0.4196, 0.2863, 0.1686));
		m_leaf_material = material(shader, leaf_texture, vec3(0.2,0.8,0.4));
		m_terrain_material = material(shader, terrain_texture, vec3(0.251, 0.161, 0.020));
		m_basic_water_material = material(shader, basic_water_texture, vec3(0,0.2,0.8));
		m_water_sim_material = material(shader, water_sim_texture, vec3(0,0.2,0.8));
	}
	else if(style == "Sketched")
	{
		//Load texture for sketched shader
		GLuint stroke_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//strokeMap.png")).uploadTexture();
		m_firefly_material = material(shader, stroke_texture, vec3(1,1,0));
		m_trunk_material = material(shader, stroke_texture, vec3(0.4196, 0.2863, 0.1686));
		m_leaf_material = material(shader, stroke_texture, vec3(0.2,0.8,0.4));
		m_terrain_material = material(shader, stroke_texture, vec3(0.251, 0.161, 0.020));
		m_basic_water_material = material(shader, stroke_texture, vec3(0,0.2,0.8));
		m_water_sim_material = material(shader, stroke_texture, vec3(0,0.2,0.8));
	}
	else if(style == "Pixel")
	{
		//Load texture for pixel shader
		GLuint pixel_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//strokeMap.png")).uploadTexture();
		m_firefly_material = material(shader, pixel_texture, vec3(1,1,0));
		m_trunk_material = material(shader, pixel_texture, vec3(0.4196, 0.2863, 0.1686));
		m_leaf_material = material(shader, pixel_texture, vec3(0.2,0.8,0.4));
		m_terrain_material = material(shader, pixel_texture, vec3(0.251, 0.161, 0.020));
		m_basic_water_material = material(shader, pixel_texture, vec3(0,0.2,0.8));
		m_water_sim_material = material(shader, pixel_texture, vec3(0,0.2,0.8));
	}
	else
	{
		shader = buildVertAndFragShader(CGRA_SRCDIR + std::string("//res//shaders//color"));
		m_firefly_material = material(shader);
		m_trunk_material = material(shader);
		m_leaf_material = material(shader);
		m_terrain_material = material(shader);
		m_basic_water_material = material(shader);
		m_water_sim_material = material(shader);
	}
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

	//Check for inputs
	m_controller.checkedPressed();

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
	fireflies.draw(view, proj, m_firefly_material);
}


void Application::renderTrees(const mat4 &view, const mat4 proj){
	trees.draw(view, proj, m_trunk_material, m_leaf_material);
}

void Application::renderWater(const mat4 &view, const mat4 proj){
	if(water_sim_enabled)
	{
		m_water.draw(view, proj, m_water_sim_material);
	}
	else
	{
		basic_water.draw(view, proj, m_basic_water_material);
	}
}

void Application::renderTerrain(const mat4 &view, const mat4 proj){
	m_terrain.draw(view, proj, m_terrain_material);
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
		m_water.simulate();
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

	int shaderWindowHeight = 75; //can change height here if you add more controls
	int shaderWindowPos = fireflyWindowPos + fireflyWindowHeight + gap;
	renderShaderGUI(shaderWindowHeight, shaderWindowPos);

	int treesWindowHeight = 125; //can change height here if you add more controls
	int treesWindowPos = shaderWindowPos + shaderWindowHeight + gap;
	trees.renderGUI(m_terrain, treesWindowHeight, treesWindowPos);

	int waterWindowHeight = 230; //can change height here if you add more controls
	int waterWindowPos = treesWindowPos + treesWindowHeight + gap;
	renderWaterGUI(waterWindowHeight, waterWindowPos);
	//TODO steal the renderWaterGUI function from down below and move it into the forest class, 
	//m_water.renderGUI(treesWindowHeight, treesWindowPos);
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
	float moveDirection = sign(yoffset) * pow(1.1f, -yoffset);
	m_camera.move(vec3(0,0,2) * moveDirection);
}



void Application::keyCallback(int key, int scancode, int action, int mods) {
	m_controller.keyCallback(key, scancode, action, mods);
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
			if (mode == "water_sim_enabled=") {
				settingsLine >> water_sim_enabled;
				std::cout << "Set water simulation default to " << (water_sim_enabled ? "On" : "Off") << std::endl;
			}
			else if (mode == "framerate_limit=") {
				settingsLine >> max_frames;
				std::cout << "Set frame rate limit to " << max_frames << std::endl;
			}
			else if (mode == "tree_recursion_depth=") {
				settingsLine >> trees.recursion_depth;
				std::cout << "Set tree recursion depth to " << trees.recursion_depth << std::endl;
			}
			else if (mode == "other_things") {
				std::string placeHolder;
				settingsLine >> placeHolder;
			}
		}
	}
}
