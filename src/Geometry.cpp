#include "Geometry.hpp"

GeometryData GeometryData::Cube(float length) {
    length = length / 2;
    std::vector<glm::vec3> vertices = {{
        {-length, -length,  length}, // bottom, then clockwise
        { length, -length,  length},
        { length, -length, -length},
        {-length, -length, -length},
        {-length,  length,  length}, // top, then clockwise
        { length,  length,  length},
        { length,  length, -length},
        {-length,  length, -length},
    }};

    std::vector<uint8_t> indices{{
        0, 1, 4, // front
        1, 5, 4,
        2, 3, 6, // back
        3, 7, 6,
        3, 0, 7, // left
        0, 4, 7,
        1, 2, 5, // right
        2, 6, 5,
        4, 5, 7, // top
        5, 6, 7,
        0, 1, 3, // bottom
        1, 2, 3
    }};

    return { vertices, indices };
}

Geometry::Geometry(const GeometryData& data, glm::mat4 model, std::shared_ptr<Shader> shader) {
    num_vertices = data.vertices.size();
    num_indices = data.indices.size();

    m_modelMatrix = model;
    m_shader = shader;

    glGenVertexArrays(1, &m_vao); 
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position attribute

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint8_t) * data.indices.size(), data.indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Geometry::~Geometry() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}


void Geometry::draw() const {
    m_shader->use();
    m_shader->setUniform("model", m_modelMatrix);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_BYTE, 0);
    glBindVertexArray(0);
}

