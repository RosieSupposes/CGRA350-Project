#pragma once

#include "opengl.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

namespace {
    struct draw_mesh_vertex {
        glm::vec3 pos{ 0 };
        glm::vec3 norm{ 0 };
        glm::vec2 uv{ 0 };
    };

    GLuint compileDrawVAO(const float *vertices, int vcount, const unsigned int *indices, int icount) {
        GLuint vao, vbo, ibo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(float), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(draw_mesh_vertex), (void *)(offsetof(draw_mesh_vertex, pos)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(draw_mesh_vertex), (void *)(offsetof(draw_mesh_vertex, norm)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(draw_mesh_vertex), (void *)(offsetof(draw_mesh_vertex, uv)));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * icount, indices, GL_STATIC_DRAW);
        glBindVertexArray(0);
        return vao;
    }
}


class Particle {
    public:
        float pressure;
        float density;
        float viscosity;
        float radius;
        float range;
        float gravity;
        float mass;

        glm::vec3 currentForce;
        glm::vec3 position;
        glm::vec3 velocity;

        glm::vec3 color;

        std::vector<Particle*> neighbours;

        Particle();
        void simulate(float timestep);
        void draw(const glm::mat4 &view, const glm::mat4 &proj, GLuint shader);\

        float compute_density();
        float compute_pressure(float density);
};

void drawSphere();
