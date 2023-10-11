
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
#include "Other/RNG.hpp"


using namespace std;
using namespace cgra;
using namespace glm;


Application::Application(GLFWwindow *window) : m_window(window) {
	const auto now = std::chrono::system_clock::now();
	auto time = now.time_since_epoch();
	m_seed = time.count() % 1000;
	std::cout << "seed: " << m_seed << std::endl; 
	readSettings();
	loadShadersAndMaterials();
	changeStyle(styles[0]);

	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
    // glGenFramebuffers(1, &framebuffer);
    
    // glGenTextures(1, &depthMap);
    // glBindTexture(GL_TEXTURE_2D, depthMap);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	// glDrawBuffer(GL_NONE);
	// glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
	load_scene_objects();
	
	m_camera = camera();
	m_firefly_material.updateCam(m_camera.m_position);
	m_trunk_material.updateCam(m_camera.m_position);
	m_leaf_material.updateCam(m_camera.m_position);
	m_terrain_material.updateCam(m_camera.m_position);
	m_basic_water_material.updateCam(m_camera.m_position);
	m_water_sim_material.updateCam(m_camera.m_position);
	
	m_controller = keyboard_controller();
	m_controller.m_camera = &m_camera;
}

void Application::load_scene_objects()
{
	fireflies = firefly_cluster(fireflyCount);
	m_terrain = terrain(CGRA_SRCDIR + std::string("//res//assets//land.obj"), scale(mat4(1), vec3(8)));
	// trees.push_back(forest(m_terrain, translate(mat4(1), vec3(-30,0,-30)),treeCount, recursion_depth, std::string(tree_styles[1])));
	// trees.push_back(forest(m_terrain, translate(mat4(1), vec3(-30,0,30)),treeCount, recursion_depth, std::string(tree_styles[1])));
	// trees.push_back(forest(m_terrain, translate(mat4(1), vec3(30,0,-30)),treeCount, recursion_depth, std::string(tree_styles[1])));
	// trees.push_back(forest(m_terrain, translate(mat4(1), vec3(30,0,30)),treeCount, recursion_depth, std::string(tree_styles[1])));
	trees.push_back(forest(m_terrain, mat4(1),treeCount, recursion_depth, std::string(tree_styles[1])));
	basic_water = basic_model(CGRA_SRCDIR + std::string("//res//assets//simple_water.obj"), vec3(0.0,0.9,0.9));
	m_water = water_sim(&water_sim_enabled); 

}

void Application::loadShadersAndMaterials(){

	m_shaderPBR = buildVertAndFragShader(CGRA_SRCDIR + std::string("//res//shaders//PBR"));
	m_shaderSketched = buildVertAndFragShader(CGRA_SRCDIR + std::string("//res//shaders//Sketched"));
	m_shaderPixel = buildVertAndFragShader(CGRA_SRCDIR + std::string("//res//shaders//Pixel"));
	m_strokeMap = rgba_image(CGRA_SRCDIR + std::string("//res//textures//strokeMap.png")).uploadTexture();
	m_pixelDitherMap = rgba_image(CGRA_SRCDIR + std::string("//res//textures//pixelDitherMap.jpg")).uploadTexture();

	firefly_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//fireflyPBR.png")).uploadTexture();
	trunk_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//trunkPBR.jpg")).uploadTexture();
	leaf_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//leafPBR.png")).uploadTexture();
	terrain_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//terrainPBR.jpg")).uploadTexture();
	basic_water_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//water.jpg")).uploadTexture();
	water_sim_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//water.jpg")).uploadTexture();


	m_depth_material = material(buildVertAndFragShader(CGRA_SRCDIR + std::string("//res//shaders//depth")));
	m_firefly_material = material(m_shaderPixel, firefly_texture, m_strokeMap, vec3(1,1,0.2), m_sketchedC, m_metallic, m_roughness);
	m_trunk_material = material(m_shaderPBR, trunk_texture, m_strokeMap, vec3(0.4196, 0.2863, 0.1686),m_sketchedC, m_metallic, m_roughness);
	m_leaf_material = material(m_shaderPBR, leaf_texture, m_strokeMap, vec3(0.2,0.8,0.4), m_sketchedC, m_metallic, m_roughness);
	m_terrain_material = material(m_shaderPBR, terrain_texture, m_strokeMap, vec3(0.251, 0.161, 0.020), m_sketchedC, m_metallic, m_roughness);
	m_basic_water_material = material(m_shaderPBR, basic_water_texture, m_strokeMap, vec3(0,0.2,0.8), m_sketchedC, m_metallic, m_roughness);
	m_water_sim_material = material(m_shaderPBR, water_sim_texture, m_strokeMap, vec3(0,0.2,0.8), m_sketchedC, m_metallic, m_roughness);

	m_firefly_material.updateLight(m_lightPosition, m_lightPower);
	m_trunk_material.updateLight(m_lightPosition, m_lightPower);
	m_leaf_material.updateLight(m_lightPosition, m_lightPower);
	m_terrain_material.updateLight(m_lightPosition, m_lightPower);
	m_basic_water_material.updateLight(m_lightPosition, m_lightPower);
	m_water_sim_material.updateLight(m_lightPosition, m_lightPower);
}

void Application::changeStyle(const char * type){
	string style = std::string(type);
	if(style == "Pixel")
	{
		changeShaders(m_shaderPixel);
		changeEffectTextures(m_pixelDitherMap);
	}
	else if (style == "Sketched")
	{
		changeShaders(m_shaderSketched);
		changeEffectTextures(m_strokeMap);
	}else
	{
		changeShaders(m_shaderPBR);
	}
}

void Application::changeShaders(GLuint shader)
{
	//m_firefly_material.setNewShader(shader);
	m_trunk_material.setNewShader(shader);
	m_leaf_material.setNewShader(shader);
	m_terrain_material.setNewShader(shader);
	m_basic_water_material.setNewShader(shader);
	m_water_sim_material.setNewShader(shader);
}
void Application::changeEffectTextures(GLuint effectTexture)
{
	m_firefly_material.changeEffectTexture(effectTexture);
	m_trunk_material.changeEffectTexture(effectTexture);
	m_leaf_material.changeEffectTexture(effectTexture);
	m_terrain_material.changeEffectTexture(effectTexture);
	m_basic_water_material.changeEffectTexture(effectTexture);
	m_water_sim_material.changeEffectTexture(effectTexture);
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

	// glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// // clear the back-buffer
	glClearColor(bg.r, bg.g, bg.b, bg.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// enable flags for normal/forward rendering
	
	glEnable(GL_DEPTH_TEST); 
	//glDepthFunc(GL_ALWAYS);

	mat4 proj = m_camera.getProjection();
	mat4 view = m_camera.getView();
	

	// helpful draw options
	if (m_show_grid) drawGrid(view, proj);
	if (m_show_axis) drawAxis(view, proj);
	glPolygonMode(GL_FRONT_AND_BACK, (m_showWireframe) ? GL_LINE : GL_FILL);
	
	//Simulate things
	simulate();

	// draw things
	//Render pass 1 to render buffer;
	// float sf = 1.01f;
	// renderTerrain(view, proj, m_depth_material);
	// renderFireflies(view, proj, m_depth_material);
	// renderWater(view, proj, m_depth_material, m_depth_material);
	// renderTrees(view, proj, m_depth_material, m_depth_material);
	
	// m_firefly_material.setDepthTexture(depthMap);
	// m_trunk_material.setDepthTexture(depthMap);
	// m_leaf_material.setDepthTexture(depthMap);
	// m_terrain_material.setDepthTexture(depthMap);
	// m_basic_water_material.setDepthTexture(depthMap);
	// m_water_sim_material.setDepthTexture(depthMap);
	
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, width, height); // set the viewport to draw to the entire window
	
	//glClearColor(bg.r, bg.g, bg.b, bg.a);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glDepthFunc(GL_LESS);
	renderWater(view, proj, m_water_sim_material, m_basic_water_material);
	renderTerrain(view, proj, m_terrain_material);
	renderTrees(view, proj, m_trunk_material, m_leaf_material);
	renderFireflies(view, proj, m_firefly_material);

    

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

void Application::renderFireflies(const mat4 &view, const mat4 proj, material& material){
	fireflies.draw(view, proj, material);
}


void Application::renderTrees(const mat4 &view, const mat4 proj, material& trunk_material, material& leaf_material){
	for(forest f : trees){
		f.draw(view, proj, trunk_material, leaf_material);
	}
}

void Application::renderWater(const mat4 &view, const mat4 proj, material& w_material, material& b_material){
	if(water_sim_enabled)
	{
		m_water.draw(view, proj, w_material);
	}
	else
	{
		basic_water.draw(view, proj, b_material);
	}
}

void Application::renderTerrain(const mat4 &view, const mat4 proj, material& material){
	m_terrain.draw(view, proj, material);
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
	for(forest f : trees){
		f.simulate();
	}
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

	if(ImGui::InputInt("Seed", &m_seed))
	{
		m_seed = glm::max(glm::min(m_seed, 1000), 0);
		RNG::resetRNG(m_seed, 0);
		load_scene_objects();
	}
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
	renderForestGUI(treesWindowHeight, treesWindowPos);

	int waterWindowHeight = 230; //can change height here if you add more controls
	int waterWindowPos = treesWindowPos + treesWindowHeight + gap;
	m_water.renderGUI(waterWindowHeight, waterWindowPos);
	//TODO steal the renderWaterGUI function from down below and move it into the forest class, 
	//m_water.renderGUI(treesWindowHeight, treesWindowPos);
}

void Application::renderShaderGUI(int height, int pos) {
	ImGui::SetNextWindowPos(ImVec2(5, pos), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, height), ImGuiSetCond_Once);
	ImGui::Begin("Style", 0);

	ImGui::Text("Style");
	if (ImGui::Combo("Style", &m_selected_style, styles, sizeof(styles) / sizeof(*styles))) {
		changeStyle(styles[m_selected_style]);
	}
	float light[3] = {m_lightPosition.x,m_lightPosition.y,m_lightPosition.z};
	bool lightChanged =false; //= ImGui::SliderFloat3("Light position", light, -50.0f, 50);
	if(lightChanged){
		m_lightPosition = vec3(light[0], light[1], light[2]);
		m_firefly_material.updateLight(m_lightPosition, m_lightPower);
		m_trunk_material.updateLight(m_lightPosition, m_lightPower);
		m_leaf_material.updateLight(m_lightPosition, m_lightPower);
		m_terrain_material.updateLight(m_lightPosition, m_lightPower);
		m_basic_water_material.updateLight(m_lightPosition, m_lightPower);
		m_water_sim_material.updateLight(m_lightPosition, m_lightPower);
	}
	bool sliderChanged = false;
	if(std::string(styles[m_selected_style]) == "PBR")
	{
		//if(ImGui::SliderFloat("Power", &m_lightPower, 0, 100, "%.3f"))
		// {
		// 	m_firefly_material.updateLight(m_lightPosition, m_lightPower);
		// 	m_trunk_material.updateLight(m_lightPosition, m_lightPower);
		// 	m_leaf_material.updateLight(m_lightPosition, m_lightPower);
		// 	m_terrain_material.updateLight(m_lightPosition, m_lightPower);
		// 	m_basic_water_material.updateLight(m_lightPosition, m_lightPower);
		// 	m_water_sim_material.updateLight(m_lightPosition, m_lightPower);
		// }
		bool sliderMChanged = ImGui::SliderFloat("Metallic", &m_metallic, 0, 1, "%.3f");
		bool sliderRChanged = ImGui::SliderFloat("Roughness", &m_roughness, 0, 1, "%.3f");
		sliderChanged = sliderMChanged || sliderRChanged;
	}
	// if(std::string(styles[m_selected_style]) == "Sketched")
	// {
	// 	sliderChanged = ImGui::SliderFloat("C value", &m_sketchedC, 0, 30, "%.2f");
	// }
	if(sliderChanged)
	{
		//m_firefly_material.update(m_metallic, m_roughness, m_sketchedC);
		m_trunk_material.update(m_metallic, m_roughness, m_sketchedC);
		m_leaf_material.update(m_metallic, m_roughness, m_sketchedC);
		m_terrain_material.update(m_metallic, m_roughness, m_sketchedC);
		m_basic_water_material.update(m_metallic, m_roughness, m_sketchedC);
		m_water_sim_material.update(m_metallic, m_roughness, m_sketchedC);
	}
	ImGui::End();
}

void Application::renderForestGUI(int height, int pos) {
    ImGui::SetNextWindowPos(ImVec2(5, pos), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, height), ImGuiSetCond_Once);
    ImGui::Begin("Trees", 0);

    ImGui::Text("Trees");
    static int selected_tree_style = 1;
    bool tree_style_changed = ImGui::Combo("Tree style", &selected_tree_style, tree_styles, sizeof(tree_styles) / sizeof(*tree_styles));
    bool tree_count_changed = ImGui::InputInt("Trees per group", &treeCount);
    bool tree_depth_changed = ImGui::InputInt("Recursion Depth", &recursion_depth);
    if (tree_style_changed || tree_count_changed || tree_depth_changed) {
        string style = std::string(tree_styles[selected_tree_style]);
        for(forest f : trees)
		{
			f.reload(m_terrain, treeCount, recursion_depth, std::string(style));
		}
    }

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
