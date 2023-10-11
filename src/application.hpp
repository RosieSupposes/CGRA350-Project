
#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp" //Is this necessary?

#include "Fireflies/firefly_cluster.hpp"
#include "Other/basic_model.hpp"
#include "Other/camera.hpp"
#include "Other/keyboard_controller.hpp"
#include "Other/material.hpp"
#include "Other/terrain.hpp"
#include "WaterSim/water_sim.hpp"
#include "Trees/forest.hpp"
#include "Other/collider.hpp"



// Main application class
//
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

	//to track randomness
	int m_seed;

	// last input
	bool m_leftMouseDown = false;
	glm::vec2 m_mousePosition;

	// drawing flags
	bool m_show_axis = false;
	bool m_show_grid = false;
	bool m_showWireframe = false;

	//Framerate limiting
	double max_frames = -1;
	std::vector<double> frames;

	//Frame stuff
	//Background colours
	vec4 bg{0.016f, 0.102f, 0.251f, 1.0f};
	GLuint framebuffer = 0;
	GLuint depthMap = 0;

	//Camera
	camera m_camera;

	//controller
	keyboard_controller m_controller;

	//Style
	vec3 m_lightPosition{0, 4, 0};
	float m_lightPower = 35.0f;
 	const char* styles[3] = { "PBR", "Sketched", "Pixel"};
	int m_selected_style = 0;
	float m_sketchedC = 4;
	float m_metallic = 0;
	float m_roughness = 0;
	GLuint m_shaderPBR = 0;
	GLuint m_shaderSketched = 0;
	GLuint m_shaderPixel = 0;
	GLuint m_strokeMap = 0;
	GLuint m_pixelDitherMap = 0;
	GLuint firefly_texture = 0;
	GLuint trunk_texture = 0;
	GLuint leaf_texture = 0;
	GLuint terrain_texture = 0;
	GLuint basic_water_texture = 0;
	GLuint water_sim_texture = 0;

	material m_depth_material;

	// geometry
		//trees
	material m_trunk_material;
	material m_leaf_material;
	std::vector<forest> trees;

	//gui fields
    int treeCount = 50;
    int recursion_depth = 3;
    const char* tree_styles[2] = { "Basic", "Complex"};

	
		//fireflies
	int fireflyCount = 100;
	material m_firefly_material;
	firefly_cluster fireflies{0};
	
		//terrain
	material m_terrain_material;
	terrain m_terrain;
	
		//water - basic
	bool water_sim_enabled = false;
	material m_basic_water_material;
	basic_model basic_water;
		//water - simulation
	float boundDamping = -0.3f;
	float restDensity = 1.0f; //restDensity (1.0)
	float gasConstant = 2.0f; //gasConstant (2.0)
	float viscosity = -0.003f; //viscosity (-0.003)
	float particleMass = 1.0f; //particleMass (1.0)
	float smoothingRadius = 1.0f; //smoothingRadius (1.0)
	float timeStep = 0.001f; //timeStep (0.001)
	material m_water_sim_material;
	water_sim m_water;

	GLuint buildVertAndFragShader(string file_head);
	
	void renderFireflies(const mat4 &view, const mat4 proj, material& material);
	void renderTrees(const mat4 &view, const mat4 proj, material& trunk_material, material& leaf_material);
	void renderWater(const mat4 &view, const mat4 proj, material& w_material, material& b_material);
	void renderTerrain(const mat4 &view, const mat4 proj, material& material);
	
	void simulate();
	void simulateFireflies();
	void simulateTrees();
	void simulateWater();

	void WrapUpFrame(double start_time);

	void adjustFocalPoint(vec3 amount, bool setAbsolute);
	
	void readSettings();
	void changeStyle(const char* type);
	void changeShaders(GLuint shader);
	void changeEffectTextures(GLuint effectTexture);
	void loadShadersAndMaterials();
	void load_scene_objects();

public:
	// setup
	Application(GLFWwindow *);

	// disable copy constructors (for safety)
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	// rendering callbacks (every frame)
	void render();
	void renderGUI();

	void renderShaderGUI(int height, int pos);
	void renderForestGUI(int height, int pos);
	void renderWaterGUI(int height, int pos);

	// input callbacks
	void cursorPosCallback(double xpos, double ypos);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void keyCallback(int key, int scancode, int action, int mods);
	void charCallback(unsigned int c);
};
