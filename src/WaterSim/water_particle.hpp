#pragma once

#include "opengl.hpp"
#include "Other/material.hpp"
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


struct Particle {
    public:
        float pressure = 0.0f;
        float density = 0.0f;

        glm::vec3 position = glm::vec3(0,0,0);
        glm::vec3 velocity  = glm::vec3(0,0,0);

        std::vector<Particle*> neighbours;


        void simulate(float dt);
        void draw(const glm::mat4 &view, const glm::mat4 &proj, material &material);\
};

void drawSphere();
