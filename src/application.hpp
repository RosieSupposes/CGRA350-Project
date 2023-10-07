
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



// Main application class
//
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

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

	//Camera
	camera m_camera;

	//controller
	keyboard_controller m_controller;

	//Style
	const char* styles[3] = { "PBR", "Sketched", "Pixel"};

	// geometry
		//trees
	int treeCount = 20;
	int recursion_depth = 2;
	material m_trunk_material;
	material m_leaf_material;
	forest trees;
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
	
	void renderFireflies(const glm::mat4 &view, const glm::mat4 proj);
	void renderTrees(const glm::mat4 &view, const glm::mat4 proj);
	void renderWater(const glm::mat4 &view, const glm::mat4 proj);
	void renderTerrain(const glm::mat4 &view, const glm::mat4 proj);
	
	void simulate();
	void simulateFireflies();
	void simulateTrees();
	void simulateWater();

	void WrapUpFrame(double start_time);

	void adjustFocalPoint(vec3 amount, bool setAbsolute);
	
	void readSettings();
	void loadShaders(const char* type);

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
	void renderTreesGUI(int height, int pos);
	void renderWaterGUI(int height, int pos);

	// input callbacks
	void cursorPosCallback(double xpos, double ypos);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void keyCallback(int key, int scancode, int action, int mods);
	void charCallback(unsigned int c);
};
