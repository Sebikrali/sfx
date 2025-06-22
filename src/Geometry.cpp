#include "Geometry.hpp"

GeometryData GeometryData::Plane(float length) {
    float l = length / 2;
    std::vector<glm::vec3> vertices {
        {-l, 0.0f, l},
        {l,  0.0f, l},
        {l,  0.0f, -l},
        {-l, 0.0f, -l}
    };
    std::vector<uint32_t> indices {
        0, 1, 2,
        1, 2, 0,
        2, 3, 0,
        3, 0, 2
    };

    return  { vertices, indices };
}

GeometryData GeometryData::Cube(float length) {
    float l = length / 2;
    std::vector<glm::vec3> vertices {
        {-l, -l,  l}, // bottom, then clockwise
        { l, -l,  l},
        { l, -l, -l},
        {-l, -l, -l},
        {-l,  l,  l}, // top, then clockwise
        { l,  l,  l},
        { l,  l, -l},
        {-l,  l, -l},
    };
    std::vector<uint32_t> indices {
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
        0, 2, 1, // bottom
        3, 2, 0
    };

    return { vertices, indices };
}

GeometryData GeometryData::Rectangle(float width, float length, float height) {
    float w = width / 2;
    float l = length / 2;
    float h = height / 2;
    std::vector<glm::vec3> vertices {
        {-w, -h,  l}, // bottom, then clockwise
        { w, -h,  l},
        { w, -h, -l},
        {-w, -h, -l},
        {-w,  h,  l}, // top, then clockwise
        { w,  h,  l},
        { w,  h, -l},
        {-w,  h, -l},
    };
    std::vector<uint32_t> indices {
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
        0, 2, 1, // bottom
        3, 2, 0
    };

    return { vertices, indices };
}

GeometryData GeometryData::Cylinder(float radius, float height, int segments) {
    std::vector<glm::vec3> vertices;
    vertices.reserve(2 + segments * 2);

    for (int i = 1; i >= -1; i -= 2) {
        vertices.emplace_back(0.0f, height * i, 0.0f);
        float factor = (2 * std::numbers::pi) / segments;
        for (int j = 0; j < segments; j++) {
            vertices.emplace_back(cos(factor * j), height * i, sin(factor * j));
        }
    }

    std::vector<uint32_t> indices;
    indices.reserve(2 * segments * 3 + segments * 6);

    // Layout:
    // [0] top middle
    // [1]-[segments] top circle
    // [segments+1] bottom middle
    // [segments+2]-[2 + segments * 2] bottom circle

    int i = 1;
    for (; i < segments; i++) {
        indices.emplace_back(i);
        indices.emplace_back(0);
        indices.emplace_back(i + 1);
    }
    indices.emplace_back(i);
    indices.emplace_back(0);
    indices.emplace_back(1);

    i += 2;
    for (; i < (1 + segments * 2); i++) {
        indices.emplace_back(segments + 1);
        indices.emplace_back(i);
        indices.emplace_back(i + 1);
    }
    indices.emplace_back(segments + 1);
    indices.emplace_back(i);
    indices.emplace_back(segments + 2);

    for (int i = 1; i < segments; i++) {
        indices.emplace_back(i);
        indices.emplace_back(i + 1);
        indices.emplace_back(i + segments + 2);

        indices.emplace_back(i + segments + 2);
        indices.emplace_back(i + segments + 1);
        indices.emplace_back(i);
    }
    indices.emplace_back(segments);
    indices.emplace_back(1);
    indices.emplace_back(segments + 2);

    indices.emplace_back(segments + 2);
    indices.emplace_back(1 + segments * 2);
    indices.emplace_back(segments);

    return { vertices, indices };
}

GeometryData GeometryData::Sphere(float radius, int slices, int stacks) {
    std::vector<glm::vec3> vertices;
    vertices.reserve(2 + slices * stacks);

    vertices.emplace_back(0.0f, radius, 0.0f);
    vertices.emplace_back(0.0f, -radius, 0.0f);

    double theta = 2 * std::numbers::pi / slices;
    double phi = std::numbers::pi / (stacks + 1); // half circle only
    for (int i = 1; i < (stacks + 1); i++) {
        for (int j = 0; j < slices; j++) {
            vertices.emplace_back(
                radius * cos(theta * j) * sin(phi * i),
                radius * cos(phi * i),
                radius * sin(theta * j) * sin(phi * i)
            );
        }
    }

    std::vector<uint32_t> indices;
    indices.reserve(slices * stacks * 6);

    // Top
    int count = 2;
    for (; count < slices + 1; count++) {
        indices.emplace_back(0);
        indices.emplace_back(count + 1);
        indices.emplace_back(count);
    }
    indices.emplace_back(0);
    indices.emplace_back(2);
    indices.emplace_back(count);
    count++;

    for (int i = 1; i < stacks; i++) {
        for (int j = 0; j < slices - 1; j++) {
            indices.emplace_back(count);
            indices.emplace_back(count - slices);
            indices.emplace_back(count + 1);

            indices.emplace_back(count + 1);
            indices.emplace_back(count - slices);
            indices.emplace_back(count - slices + 1);
            count++;
        }
        indices.emplace_back(count);
        indices.emplace_back(count - slices);
        indices.emplace_back(count - slices + 1);

        indices.emplace_back(count - slices + 1);
        indices.emplace_back(count - slices);
        indices.emplace_back(count - 2 * slices + 1);
        count++;
    }

    // Bottom
    count -= slices;
    for (int i = 0; i < slices - 1; i++) {
        indices.emplace_back(1);
        indices.emplace_back(count);
        indices.emplace_back(count + 1);
        count++;
    }
    indices.emplace_back(1);
    indices.emplace_back(count);
    indices.emplace_back(count - slices + 1);

    return  { vertices, indices };
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * data.indices.size(), data.indices.data(), GL_STATIC_DRAW);

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
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

