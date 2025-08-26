#pragma once

#include "pch.h"
#include "Shader.hpp"

struct MeshData {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<uint32_t> indices;

    MeshData() = delete;
    MeshData(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs, const std::vector<uint32_t>& indices) : vertices(vertices), normals(normals), uvs(uvs), indices(indices) {}
    MeshData(const aiMesh* mesh);

    static MeshData Default();
    static MeshData Plane(float length = 1.0f);
    /**
     * @brief Creates a cube with middle point (0,0,0).
     * @param length The length of one side.
     */
    static MeshData Cube(float length = 1.0f);
    static MeshData Rectangle(float width = 2.0f, float length = 1.0f, float height = 1.0f);
    static MeshData Cylinder(float radius = 1.0f, float height = 1.0f, int segments = 10);
    /**
     * @brief Generates a UV sphere mesh.
     * @param radius The radius of the sphere.
     * @param slices Number of vertical subdivisions.
     * @param stacks Number of horizontal subdivisions.
     */
    static MeshData Sphere(float radius = 1.0f, int slices = 32, int stacks = 16);
    // NOTE: Maybe also add an Icosphere
};


struct Mesh {
    bool initialized = false;
    glm::mat4 m_modelMatrix;
    int num_vertices, num_indices;
    bool hasNormals = false;
    bool hasUVs = false;

    Mesh() = default;
    Mesh(const MeshData& data, glm::mat4 model);
    ~Mesh();

    /**
     * @brief Creates a default cube mesh with sidelength `1`.
     */
    static std::shared_ptr<Mesh> Default();

    void draw(std::shared_ptr<Shader> shader) const;

    // TODO: Add a setMesh method (the default constructor doesn't make sense without one)

private:
    GLuint m_vao;
    GLuint m_vbo; // NOTE: Maybe rename to vboPos or something similar for consistency
    GLuint m_vboNormals;
    GLuint m_vboUVs;
    GLuint m_ebo;
};
