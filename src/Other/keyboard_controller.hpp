#pragma once

// glm
#include <string>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>

// project
#include "opengl.hpp"
#include "camera.hpp"

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

class keyboard_controller{
private:
    std::unordered_map<keys,bool> keyMap;
	
	void setKeysOff();
    
public:
	camera *m_camera;
	keyboard_controller();
    void keyCallback(int key, int scancode, int action, int mods);
	void checkedPressed();
}; 
