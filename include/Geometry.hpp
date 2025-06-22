#pragma once

#include "pch.h"
#include "Shader.hpp"

struct GeometryData {
    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;

    GeometryData(const std::vector<glm::vec3>& vertices, std::vector<uint32_t> indices) : vertices(vertices), indices(indices) {}

    static GeometryData Plane(float length = 1.0f);
    static GeometryData Cube(float length = 1.0f);
    static GeometryData Rectangle(float width = 2.0f, float length = 1.0f, float height = 1.0f);
    static GeometryData Cylinder(float radius = 1.0f, float height = 1.0f, int segments = 10);
    /**
     * @brief Generates a UV sphere mesh.
     * @param radius The radius of the sphere.
     * @param slices Number of vertical subdivisions.
     * @param stacks Number of horizontal subdivisions.
     */
    static GeometryData Sphere(float radius = 1.0f, int slices = 32, int stacks = 16);
    // NOTE: Maybe also add an Icosphere
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
