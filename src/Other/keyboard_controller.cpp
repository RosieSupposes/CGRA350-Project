#include "keyboard_controller.hpp"


using namespace glm;

keyboard_controller::keyboard_controller()
{
	setKeysOff();
}

void keyboard_controller::setKeysOff()
{
	keyMap[W] = false;
	keyMap[UP] = false;
	keyMap[A] = false;
	keyMap[LEFT] = false;
	keyMap[S] = false;
	keyMap[DOWN] = false;
	keyMap[D] = false;
	keyMap[RIGHT] = false;
}

void keyboard_controller::keyCallback(int key, int scancode, int action, int mods){
	switch (action)
	{
	case 0:
		keyMap[(keys)key] = false;
		break;
	case 1:
		/* code */
		keyMap[(keys)key] = true;
		break;
	
	default:
		break;
	}
	std::cout << "Key:" << key << std::endl
	<< "scancode" << scancode << std::endl
	<< "action" << action << std::endl
	<< "mods" << mods << std::endl
	<< std::endl;
}

void keyboard_controller::checkedPressed()
{
	bool keyPressed = false;
	if(keyMap[W] || keyMap[UP]){
		m_camera->move(vec3(0,0,1));
		std::cout << "Moved Cam up" << std::endl;
	}
	if(keyMap[A] || keyMap[LEFT]){
		m_camera->move(vec3(1,0,0));
		std::cout << "Moved Cam left" << std::endl;
	}
	if(keyMap[S] || keyMap[DOWN]){
		m_camera->move(vec3(0,0,-1));
	}
	if(keyMap[D] || keyMap[RIGHT]){
		m_camera->move(vec3(-1,0,0));
	}
	if(keyPressed == false){
		m_camera->reduceVelocity();
	}
}
