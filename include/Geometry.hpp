#pragma once

#include "pch.h"
#include "Shader.hpp"
#include <memory>

struct GeometryData {
    std::vector<glm::vec3> vertices;
    std::vector<uint8_t> indices;

    GeometryData(const std::vector<glm::vec3>& vertices, std::vector<uint8_t> indices) : vertices(vertices), indices(indices) {}

    static GeometryData Cube(float length);
};

struct Geometry {
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;

    int num_vertices;
    int num_indices;

    std::shared_ptr<Shader> m_shader;
    glm::mat4 m_modelMatrix;

    Geometry(const GeometryData& data, glm::mat4 model, std::shared_ptr<Shader> shader);
    ~Geometry();

    void draw() const;
};
