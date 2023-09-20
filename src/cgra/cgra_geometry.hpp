
#pragma once

#include <glm/glm.hpp>

namespace cgra {

	// sets up a shader and draws an axis straight to the current framebuffer
	void drawAxis(const glm::mat4 &view, const glm::mat4 &proj);

	// sets up a shader and draws a grid straight to the current framebuffer
	void drawGrid(const glm::mat4 &view, const glm::mat4 &proj);
}
