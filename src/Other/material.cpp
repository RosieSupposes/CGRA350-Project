#include "material.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


using namespace glm;

material::material(){

}

material::material(GLuint shader)
{
    m_shader = shader;
    isEffectShader = true;
}

material::material(GLuint shader, glm::vec3 color)
{
    m_shader = shader;
    m_color = color;
}

material::material(GLuint shader, GLuint texture, GLuint effectTexture, glm::vec3 color, float metallic, float roughness)
{
    m_shader = shader;
    m_texture = texture;
    m_effectTexture = effectTexture;
    m_color = color;
    //m_sketchedC = sketchedC;
    m_metallic = metallic;
    m_roughness = roughness;
}
void material::setNewShader(GLuint shader)
{
    m_shader = shader;
}

void material::changeEffectTexture(GLuint effectTexture)
{
    m_effectTexture = effectTexture;
}

void material::setDepthTexture(GLuint &depthTexture)
{
    m_isDepthTextureSet = true;
    m_depthTexture = depthTexture;
}

void material::load(const mat4 &mView, const mat4 &proj)
{
    glUseProgram(m_shader); // load shader and variables
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uViewMatrix"), 1, false, value_ptr(mView));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uModelMatrix"), 1, false, value_ptr(mat4(1)));
    glUniform1i(glGetUniformLocation(m_shader, "uTexture"), 0);
    if(isEffectShader)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        return;
    }
    glUniform1i(glGetUniformLocation(m_shader, "uEffectTexture"), 1);
    glUniform3fv(glGetUniformLocation(m_shader, "uColor"), 1, value_ptr(m_color));
    glUniform3fv(glGetUniformLocation(m_shader, "uCameraPosition"), 1, value_ptr(m_cameraPosition));
    glUniform3fv(glGetUniformLocation(m_shader, "uLightPosition"), 1, value_ptr(m_lightPosition));
    glUniform3fv(glGetUniformLocation(m_shader, "uLightColour"), 1, value_ptr(vec3(1)));
    glUniform1f(glGetUniformLocation(m_shader, "uLightPower"), m_lightPower);
    glUniform1f(glGetUniformLocation(m_shader, "uBrightness"), 1.0f);
    glUniform1f(glGetUniformLocation(m_shader, "uMetallic"), m_metallic);
    glUniform1f(glGetUniformLocation(m_shader, "uRoughness"), m_roughness);
    //glUniform1f(glGetUniformLocation(m_shader, "uC"), m_sketchedC);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_effectTexture);
}

void material::load(const mat4 &model, const mat4 &view, const mat4 &proj)
{
    glUseProgram(m_shader); // load shader and variables
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uViewMatrix"), 1, false, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "uModelMatrix"), 1, false, value_ptr(model));
    glUniform1i(glGetUniformLocation(m_shader, "uTexture"), 0);
    if(isEffectShader)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        return;
    }
    glUniform1i(glGetUniformLocation(m_shader, "uEffectTexture"), 1);
    glUniform3fv(glGetUniformLocation(m_shader, "uColor"), 1, value_ptr(m_color));
    glUniform3fv(glGetUniformLocation(m_shader, "uCameraPosition"), 1, value_ptr(m_cameraPosition));
    glUniform3fv(glGetUniformLocation(m_shader, "uLightPosition"), 1, value_ptr(m_lightPosition));
    glUniform3fv(glGetUniformLocation(m_shader, "uLightColour"), 1, value_ptr(vec3(1)));
    glUniform1f(glGetUniformLocation(m_shader, "uLightPower"), m_lightPower);
    glUniform1f(glGetUniformLocation(m_shader, "uBrightness"), 1.0f);
    glUniform1f(glGetUniformLocation(m_shader, "uMetallic"), m_metallic);
    glUniform1f(glGetUniformLocation(m_shader, "uRoughness"), m_roughness);
    //glUniform1f(glGetUniformLocation(m_shader, "uC"), m_sketchedC);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_effectTexture);
    
}

void material::update(float metallic, float roughness)
{
	m_metallic = metallic;
	m_roughness = roughness;
	//m_sketchedC = cValue;
}
void material::updateLight(glm::vec3 &lightPosition, float lightPower)
{
	m_lightPosition = lightPosition;
    m_lightPower = lightPower;
}
void material::updateCam(glm::vec3 &cameraPosition)
{
	m_cameraPosition = cameraPosition;
}