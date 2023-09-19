
#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"
#include "cgra/cgra_mesh.hpp" //Is this necessary?

#include "firefly.hpp"
#include "simple_water.hpp"
#include "water_sim.hpp"
#include "tree.hpp"



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

	// geometry
		//trees
	int treeCount = 20;
	GLuint tree_shader = 0;
	std::vector<tree> trees;
	
		//fireflies
	int fireflyCount = 100;
	GLuint firefly_shader = 0;
	std::vector<firefly> fireflies;
	
		//water
	bool water_sim_enabled = false;
	GLuint basic_water_shader = 0;
	simple_water basic_water{0};
	
		//watersim
	GLuint water_shader = 0;
	water_sim water{0};

	
	void renderFireflies(const glm::mat4 &view, const glm::mat4 proj);
	void renderTrees(const glm::mat4 &view, const glm::mat4 proj);
	void renderWater(const glm::mat4 &view, const glm::mat4 proj);

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
