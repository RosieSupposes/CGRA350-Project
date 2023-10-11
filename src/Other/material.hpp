#pragma once

// glm
#include <string>
#include <glm/glm.hpp>

// project
#include "opengl.hpp"



class material{
private:
    
	GLuint m_texture = 0;
	GLuint m_effectTexture = 0;
	bool isDepthShader = false;
	bool m_isDepthTextureSet = false;
	GLuint m_depthTexture;
	glm::vec3 m_color{0.7};
	float m_metallic = 0;
	float m_roughness = 0;
	float m_sketchedC = 0;
	glm::vec3 m_lightPosition{0};
	float m_lightPower;
	glm::vec3 m_cameraPosition{0};
    
public:
	GLuint m_shader = 0;
    material();
	material(GLuint shader);
	material(GLuint shader, GLuint texture, GLuint effectTexture, glm::vec3 color, float sketchedC, float metallic, float roughness);
	void setNewShader(GLuint shader);
	void setDepthTexture(GLuint &depthTexture);
	void changeEffectTexture(GLuint effectTexture);
	void update(float metallic, float roughness, float cValue);
	void updateLight(glm::vec3 &lightPosition, float value);
	void updateCam(glm::vec3 &cameraPosition);
	void load(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj);
}; 
