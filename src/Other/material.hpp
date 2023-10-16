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
	bool isEffectShader = false;
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
	material(GLuint shader, glm::vec3 color);
	material(GLuint shader, GLuint texture, GLuint effectTexture, glm::vec3 color, float metallic, float roughness);
	void setNewShader(GLuint shader);
	void setDepthTexture(GLuint &depthTexture);
	void changeTexture(GLuint uTexture);
	void changeEffectTexture(GLuint effectTexture);
	void update(float metallic, float roughness);
	void updateLight(glm::vec3 &lightPosition, float value);
	void updateCam(glm::vec3 &cameraPosition);
	void load(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj);
	void load(const glm::mat4 &mView, const glm::mat4 &proj);
}; 
