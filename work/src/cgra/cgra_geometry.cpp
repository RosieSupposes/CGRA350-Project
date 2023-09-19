
// std
#include <vector>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// project
#include "cgra_geometry.hpp"
#include "cgra_shader.hpp"
#include <opengl.hpp>

namespace cgra {

	void drawAxis(const glm::mat4 &view, const glm::mat4 &proj) {

		const char* axis_shader_source = R"(
	#version 330 core
	uniform mat4 uProjectionMatrix;
	uniform mat4 uModelViewMatrix;
#ifdef _VERTEX_
	flat out int v_instanceID;
	void main() {
		v_instanceID = gl_InstanceID;
	}
#endif
#ifdef _GEOMETRY_
	layout(points) in;
	layout(line_strip, max_vertices = 2) out;
	flat in int v_instanceID[];
	out vec3 v_color;
	const vec3 dir[] = vec3[](
		vec3(1, 0, 0),
		vec3(-.5, 0, 0),
		vec3(0, 1, 0),
		vec3(0, -.5, 0),
		vec3(0, 0, 1),
		vec3(0, 0, -.5)
	);
	void main() {
		v_color = abs(dir[v_instanceID[0]]);
		gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(0.0, 0.0, 0.0, 1.0);
		EmitVertex();
		v_color = abs(dir[v_instanceID[0]]);
		gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(normalize(dir[v_instanceID[0]]) * 1000, 1.0);
		EmitVertex();
		EndPrimitive();
	}
#endif
#ifdef _FRAGMENT_
	in vec3 v_color;
	out vec3 f_color;
	void main() {
		gl_FragDepth = gl_FragCoord.z - 0.000001;
		f_color = v_color;
	}
#endif)";
		static GLuint axis_shader = 0;
		if (!axis_shader) {
			shader_builder prog;
			prog.set_shader_source(GL_VERTEX_SHADER, axis_shader_source);
			prog.set_shader_source(GL_GEOMETRY_SHADER, axis_shader_source);
			prog.set_shader_source(GL_FRAGMENT_SHADER, axis_shader_source);
			axis_shader = prog.build();
		}

		glUseProgram(axis_shader);
		glUniformMatrix4fv(glGetUniformLocation(axis_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(axis_shader, "uModelViewMatrix"), 1, false, value_ptr(view));
		draw_dummy(6);
	}


	void drawGrid(const glm::mat4 &view, const glm::mat4 &proj) {

		const char* grid_shader_source = R"(
	#version 330 core
	uniform mat4 uProjectionMatrix;
	uniform mat4 uModelViewMatrix;
#ifdef _VERTEX_
	flat out int v_instanceID;
	void main() {
		v_instanceID = gl_InstanceID;
	}
#endif
#ifdef _GEOMETRY_
	layout(points) in;
	layout(line_strip, max_vertices = 2) out;
	flat in int v_instanceID[];
	void main() {
		gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(v_instanceID[0] - 10, 0, -10, 1);
		EmitVertex();
		gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(v_instanceID[0] - 10, 0, 10, 1);
		EmitVertex();
		EndPrimitive();
	}
#endif
#ifdef _FRAGMENT_
	out vec3 f_color;
	void main() {
		f_color = vec3(0.5, 0.5, 0.5);
	}
#endif)";
		static GLuint grid_shader = 0;
		if (!grid_shader) {
			shader_builder prog;
			prog.set_shader_source(GL_VERTEX_SHADER, grid_shader_source);
			prog.set_shader_source(GL_GEOMETRY_SHADER, grid_shader_source);
			prog.set_shader_source(GL_FRAGMENT_SHADER, grid_shader_source);
			grid_shader = prog.build();
		}

		const glm::mat4 rot = glm::rotate(glm::mat4(1), glm::pi<float>() / 2.f, glm::vec3(0, 1, 0));

		glUseProgram(grid_shader);
		glUniformMatrix4fv(glGetUniformLocation(grid_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(grid_shader, "uModelViewMatrix"), 1, false, value_ptr(view));
		draw_dummy(21);
		glUniformMatrix4fv(glGetUniformLocation(grid_shader, "uModelViewMatrix"), 1, false, value_ptr(view * rot));
		draw_dummy(21);
	}
}
