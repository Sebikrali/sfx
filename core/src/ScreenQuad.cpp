#include "sfx/ScreenQuad.hpp"


ScreenQuad::ScreenQuad() {
}

ScreenQuad::ScreenQuad(glm::vec4 bounds) {
    float vertices[] = {
        // pos        uvs
        bounds.x, bounds.y,   1.0f, 1.0f,
        bounds.z, bounds.y,   0.0f, 1.0f,
        bounds.z, bounds.w,   0.0f, 0.0f,
        bounds.x, bounds.w,   1.0f, 0.0f,
    };

    uint32_t indices[] = {
        0, 1, 2, 2, 3, 0
    };

    glCreateVertexArrays(1, &vao); 

    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 4);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0); 
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2);

    glCreateBuffers(1, &ebo);
    glNamedBufferData(ebo, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(vao, ebo);
}

void ScreenQuad::draw() const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

