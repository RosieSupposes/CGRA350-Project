
#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp" //Is this necessary?

#include "Fireflies/firefly_cluster.hpp"
#include "Simple/basic_model.hpp"
#include "WaterSim/water_sim.hpp"
#include "Trees/forest.hpp"



// Main application class
//
class Application {
private:
	// window
	glm::vec2 m_windowsize;
	GLFWwindow *m_window;

	// oribital camera
	float m_pitch = .86;
	float m_yaw = -.86;
	float m_distance = 20;

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

	//Style
	const char* styles[3] = { "PBR", "Sketched", "Pixel"};

	// geometry
		//trees
	int treeCount = 20;
	int recursion_depth = 2;
	GLuint tree_shader = 0;
	forest trees{0};
	
		//fireflies
	int fireflyCount = 100;
	GLuint firefly_shader = 0;
	firefly_cluster fireflies{0};
	
	//terrain
	basic_model terrain;
	
		//water
	bool water_sim_enabled = false;
	GLuint basic_water_shader = 0;
	basic_model basic_water;
	
		//watersim
	GLuint water_shader = 0;
	water_sim water;

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

	// input callbacks
	void cursorPosCallback(double xpos, double ypos);
	void mouseButtonCallback(int button, int action, int mods);
	void scrollCallback(double xoffset, double yoffset);
	void keyCallback(int key, int scancode, int action, int mods);
	void charCallback(unsigned int c);
};
