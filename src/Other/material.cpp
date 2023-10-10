#include "material.hpp"
#include <glm/gtc/type_ptr.hpp>


using namespace glm;

material::material(){

}

material::material(GLuint shader){
    m_shader = shader;
}

material::material(GLuint shader, GLuint texture){
    m_shader = shader;
    m_texture = texture;
}	

material::material(GLuint shader, GLuint texture, glm::vec3 color){
    m_shader = shader;
    m_texture = texture;
    m_color = color;
}
material::material(GLuint shader, GLuint texture, glm::vec3 color, float metallic, float roughness){
    m_shader = shader;
    m_texture = texture;
    m_color = color;
    m_metallic = metallic;
    m_roughness = roughness;
}
void material::load(const mat4 &modelview, const mat4 &proj){
    glUseProgram(m_shader); // load shader and variables
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "uModelViewMatrix"), 1, false, value_ptr(modelview));
    glUniform3fv(glGetUniformLocation(m_shader, "uColor"), 1, value_ptr(m_color));
    glUniform1f(glGetUniformLocation(m_shader, "uBrightness"), 1.0f);
    glUniform1f(glGetUniformLocation(m_shader, "uMetallic"), m_metallic);
    glUniform1f(glGetUniformLocation(m_shader, "uRoughness"), m_roughness);
}