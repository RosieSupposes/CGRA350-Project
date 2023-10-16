
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
	//std::cout << "seed: " << m_seed << std::endl; 
	readSettings();
	
	loadShaders(styles[0]);
	
	load_scene_objects();

	effectSphere = basic_model(create_sphere_builder(), scale(mat4(1), vec3(50)));
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height); 
	// // framebuffer configuration - cannot get to work
    // // -------------------------
    // glGenFramebuffers(1, &framebuffer);
    // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // // create a color attachment texture
    // glGenTextures(1, &textureColorbuffer);
    // glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    // glGenRenderbuffers(1, &rbo);
    // glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //     cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	m_camera = camera();
	m_controller = keyboard_controller();
	m_controller.m_camera = &m_camera;
}

void Application::load_scene_objects()
{
	fireflies = firefly_cluster(fireflyCount);
	m_terrain = terrain(CGRA_SRCDIR + std::string("//res//assets//land.obj"), scale(mat4(1), vec3(8)));
	trees = forest(m_terrain);
	basic_water = basic_model(CGRA_SRCDIR + std::string("//res//assets//simple_water.obj"), vec3(0.0,0.9,0.9));
	m_water = water_sim(&water_sim_enabled); 

}

void Application::loadShaders(const char * type){
	string style = std::string(type);

	string file_head = CGRA_SRCDIR + std::string("//res//shaders//") + style;
	
	shader_builder builder;
	builder.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//FF_vert.glsl"));
	builder.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//FF_frag.glsl"));
	m_firefly_material = material(
			builder.build(),
			vec3(1,1,0));
	builder = shader_builder();
	builder.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//Effect_vert.glsl"));
	builder.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//Effect_frag.glsl"));

	effectMaterial = material(builder.build());

	
	
	builder = shader_builder();
	builder.set_shader(GL_VERTEX_SHADER, CGRA_SRCDIR + std::string("//res//shaders//" + style + "_vert.glsl"));
	builder.set_shader(GL_FRAGMENT_SHADER, CGRA_SRCDIR + std::string("//res//shaders//"+ style + "_frag.glsl"));
	GLuint shader = builder.build();
	sketch_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//strokeMap.png")).uploadTexture(true);
	pixel_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//pixel.jpg")).uploadTexture(true);
	GLuint firefly_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//fireflyPBR.png")).uploadTexture();
	GLuint trunk_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//trunkPBR.jpg")).uploadTexture();
	GLuint leaf_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//leafPBR.png")).uploadTexture();
	GLuint terrain_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//terrainPBR.jpg")).uploadTexture();
	GLuint basic_water_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//water.jpg")).uploadTexture();
	GLuint water_sim_texture = rgba_image(CGRA_SRCDIR + std::string("//res//textures//water.jpg")).uploadTexture();
	//m_firefly_material = material(shader, firefly_texture, vec3(1,1,0));
	m_trunk_material = material(shader, trunk_texture, sketch_texture, vec3(0.4196, 0.2863, 0.1686), 0.5, 0.8);
	m_leaf_material = material(shader, leaf_texture, sketch_texture, vec3(0.2,0.8,0.4),0,0);
	m_terrain_material = material(shader, terrain_texture, sketch_texture, terrain_colour, 0.3, 0.8);
	m_basic_water_material = material(shader, basic_water_texture, sketch_texture, water_colour, 0.255, 0);
	m_water_sim_material = material(shader, water_sim_texture, sketch_texture, water_colour, 0.255, 0);
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
	glClearColor(skyColour.x, skyColour.y, skyColour.z, 1.0f);
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

	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	ApplyEffect();
	
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// draw things
	renderTerrain(view, proj);
	renderFireflies(view, proj);
	renderWater(view, proj);
	renderTrees(view, proj);
	WrapUpFrame(start_time);
}

void Application::ApplyEffect()
{
	// // make sure we clear the framebuffer's content
    // glClearColor(skyColour.x, skyColour.y, skyColour.z, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// if(m_selected_style == 1){
	// 	effectMaterial.m_texture = sketch_texture;
	// }
	// else if(m_selected_style == 2){
	// 	effectMaterial.m_texture = pixel_texture;
	// }
	// effectSphere.m_modelTransform = scale(mat4(1), vec3(120));
	// effectSphere.draw(m_camera.getView(), m_camera.getProjection(), effectMaterial);
	// //Update materials
	// //m_firefly_material.changeEffectTexture(textureColorbuffer);
	// m_trunk_material.changeEffectTexture(textureColorbuffer);
	// m_leaf_material.changeEffectTexture(textureColorbuffer);
	// m_terrain_material.changeEffectTexture(textureColorbuffer);
	// m_basic_water_material.changeEffectTexture(textureColorbuffer);
	// m_water_sim_material.changeEffectTexture(textureColorbuffer);
	if(m_selected_style == 1){
		m_trunk_material.changeEffectTexture(sketch_texture);
		m_leaf_material.changeEffectTexture(sketch_texture);
		m_terrain_material.changeEffectTexture(sketch_texture);
		m_basic_water_material.changeEffectTexture(sketch_texture);
		m_water_sim_material.changeEffectTexture(sketch_texture);
	}else if(m_selected_style == 2){
		m_trunk_material.changeEffectTexture(pixel_texture);
		m_leaf_material.changeEffectTexture(pixel_texture);
		m_terrain_material.changeEffectTexture(pixel_texture);
		m_basic_water_material.changeEffectTexture(pixel_texture);
		m_water_sim_material.changeEffectTexture(pixel_texture);
	}
	
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
	int mainWindowHeight = 180;
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

	int fireflyWindowHeight = 200;
	int fireflyWindowPos = mainWindowPos + mainWindowHeight + gap;
	fireflies.renderGUI(fireflyWindowHeight, fireflyWindowPos);

	int shaderWindowHeight = 75; //can change height here if you add more controls
	int shaderWindowPos = fireflyWindowPos + fireflyWindowHeight + gap;
	renderShaderGUI(shaderWindowHeight, shaderWindowPos);

	int treesWindowHeight = 125; //can change height here if you add more controls
	int treesWindowPos = shaderWindowPos + shaderWindowHeight + gap;
	trees.renderGUI(m_terrain, treesWindowHeight, treesWindowPos);

	int waterWindowHeight = 435; //can change height here if you add more controls
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
		loadShaders(styles[m_selected_style]);
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
				//std::cout << "Set water simulation default to " << (water_sim_enabled ? "On" : "Off") << std::endl;
			}
			else if (mode == "framerate_limit=") {
				settingsLine >> max_frames;
				//std::cout << "Set frame rate limit to " << max_frames << std::endl;
			}
			else if (mode == "tree_recursion_depth=") {
				settingsLine >> trees.recursion_depth;
				//std::cout << "Set tree recursion depth to " << trees.recursion_depth << std::endl;
			}
			else if (mode == "other_things") {
				std::string placeHolder;
				settingsLine >> placeHolder;
			}
		}
	}
}

mesh_builder create_sphere_builder()
{
	mesh_builder mb;
	int azimuth_divisions = 40;
	int elevation_divisions = 40;
	
	vec2 placeholder = vec2(0,0);//Replace later
	
	std::vector<vec4> points;
	float elevation_step = radians(180.0f)/elevation_divisions;
	for(int i = 1; i < elevation_divisions; i++){
		vec4 point = vec4(sin(i*elevation_step),0,cos(i*elevation_step), 1);
		points.push_back(point);
	}
	std::vector<mat4> rotations;
	float azimuth_step = radians(360.0f)/azimuth_divisions;
	for(int j = 0; j < azimuth_divisions; j++){
		rotations.push_back(rotate(mat4{1.0}, j*azimuth_step, vec3(0,0,1)));
	}
	//count = 0;
	float xCount = 0;
	for(mat4 rot : rotations){
		float x_u = xCount/azimuth_divisions;
		float yCount = 1;
		for(vec4 point : points){
			float y_v = yCount/elevation_divisions;
			vec4 rP = point*rot;
			vec3 pt = vec3(rP.x, rP.y, rP.z);
			vec3 norm = vec3(pt.x, pt.y, pt.z);
			mb.push_vertex(mesh_vertex{pt, pt, vec2(x_u, y_v)});
			yCount += 1;
		}
		xCount += 1;
	}
	mb.push_vertex(mesh_vertex{vec3(0,0,1), vec3(0,0,1), vec2(0.5,0)});
	mb.push_vertex(mesh_vertex{vec3(0,0,-1), vec3(0,0,-1), vec2(0.5,1)});
	int elevation_offset = elevation_divisions - 1;
	for(int i = 0; i < azimuth_divisions-1; i++){
		for(int j = 1; j < elevation_divisions-1; j++){
			int a = i*elevation_offset + j - 1;
			int b = a + 1;
			int c = a + elevation_offset;
			int d = c + 1;
			mb.push_index(a);
			mb.push_index(b);
			mb.push_index(c);
			mb.push_index(c);
			mb.push_index(b);
			mb.push_index(d);
		}
	}
	//final side wrap around
	for(int j = 1; j < elevation_divisions-1; j++){
		int a = (azimuth_divisions-1)*elevation_offset + j - 1;
		int b = a + 1;
		int c = j - 1;
		int d = c + 1;
		mb.push_index(a);
		mb.push_index(b);
		mb.push_index(c);
		mb.push_index(c);
		mb.push_index(b);
		mb.push_index(d);
	}
	//join ends
	//end 1
	for(int i = 0; i < azimuth_divisions-1; i++){
		int a = i*elevation_offset;
		int b = azimuth_divisions*(elevation_divisions-1);
		int c = a + elevation_offset;
		mb.push_index(a);
		mb.push_index(b);
		mb.push_index(c);
	}
	mb.push_index((azimuth_divisions-1)*elevation_offset);
	mb.push_index(azimuth_divisions*(elevation_divisions-1));
	mb.push_index(0);
	for(int i = 1; i < azimuth_divisions; i++){
		int a = i*elevation_offset-1;
		int b = azimuth_divisions*(elevation_divisions-1)+1;
		int c = a + elevation_offset;
		mb.push_index(a);
		mb.push_index(b);
		mb.push_index(c);
	}
	mb.push_index(azimuth_divisions*(elevation_divisions-1)-1);
	mb.push_index(azimuth_divisions*(elevation_divisions-1)+1);
	mb.push_index(elevation_offset-1);
	return mb;
}
