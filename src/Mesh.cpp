#include "Mesh.hpp"

MeshData MeshData::Default() {
    return MeshData::Cube();
}

MeshData MeshData::Plane(float length) {
    float l = length / 2;
    std::vector<glm::vec3> vertices {
        {-l, 0.0f, l},
        {l,  0.0f, l},
        {l,  0.0f, -l},
        {-l, 0.0f, -l}
    };

    std::vector<glm::vec3> normals {
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    };

    std::vector<glm::vec2> uvs {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    std::vector<uint32_t> indices {
        0, 1, 2,
        1, 2, 0,
        2, 3, 0,
        3, 0, 2
    };

    return  { vertices, normals, uvs, indices };
}

MeshData MeshData::Cube(float length) {
    float l = length / 2;
    std::vector<glm::vec3> vertices {
        // Front
        {-l, -l, l}, // 0
        { l, -l, l}, // 1
        { l,  l, l}, // 2
        {-l,  l, l}, // 3
        // Back
        { l, -l, -l}, // 4
        {-l, -l, -l}, // 5
        {-l,  l, -l}, // 6
        { l,  l, -l}, // 7
        // Left
        {-l, -l, -l}, // 8
        {-l, -l,  l}, // 9
        {-l,  l,  l}, // 10
        {-l,  l, -l}, // 11
        // Right
        {l, -l,  l}, // 12
        {l, -l, -l}, // 13
        {l,  l, -l}, // 14
        {l,  l,  l}, // 15
        // Top
        {-l, l,  l}, // 16
        { l, l,  l}, // 17
        { l, l, -l}, // 18
        {-l, l, -l}, // 19
        // Bottom
        {-l, -l, -l}, // 20
        { l, -l, -l}, // 21
        { l, -l,  l}, // 22
        {-l, -l,  l}, // 23
    };

    std::vector<glm::vec3> normals = {
        // Front
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        // Back
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        // Left
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        // Right
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        // Top
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        // Bottom
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
    };

    std::vector<glm::vec2> uvs;
    uvs.reserve(24);
    for (int i = 0; i < 6; i++) {
        uvs.emplace_back(0.0f, 0.0f);
        uvs.emplace_back(1.0f, 0.0f);
        uvs.emplace_back(1.0f, 1.0f);
        uvs.emplace_back(0.0f, 1.0f);
    }

    std::vector<uint32_t> indices {
        0, 1, 2, // front
        2, 3, 0,
        4, 5, 6, // back
        6, 7, 4,
        8, 9, 10, // left
        10, 11, 8,
        12, 13, 14, // right
        14, 15, 12,
        16, 17, 18, // top
        18, 19, 16,
        20, 21, 22, // bottom
        22, 23, 20
    };
     
    return { vertices, normals, uvs, indices  };
}

MeshData MeshData::Rectangle(float width, float length, float height) {
    float w = width / 2;
    float l = length / 2;
    float h = height / 2;
    std::vector<glm::vec3> vertices {
        // Front
        {-w, -h, l}, // 0
        { w, -h, l}, // 1
        { w,  h, l}, // 2
        {-w,  h, l}, // 3
        // Back
        { w, -h, -l}, // 4
        {-w, -h, -l}, // 5
        {-w,  h, -l}, // 6
        { w,  h, -l}, // 7
        // Left
        {-w, -h, -l}, // 8
        {-w, -h,  l}, // 9
        {-w,  h,  l}, // 10
        {-w,  h, -l}, // 11
        // Right
        {w, -h,  l}, // 12
        {w, -h, -l}, // 13
        {w,  h, -l}, // 14
        {w,  h,  l}, // 15
        // Top
        {-w, h,  l}, // 16
        { w, h,  l}, // 17
        { w, h, -l}, // 18
        {-w, h, -l}, // 19
        // Bottom
        {-w, -h, -l}, // 20
        { w, -h, -l}, // 21
        { w, -h,  l}, // 22
        {-w, -h,  l}, // 23
    };

    std::vector<glm::vec3> normals = {
        // Front
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        // Back
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        // Left
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        // Right
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        // Top
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        // Bottom
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
    };

    std::vector<glm::vec2> uvs;
    uvs.reserve(24);
    for (int i = 0; i < 6; i++) {
        uvs.emplace_back(0.0f, 0.0f);
        uvs.emplace_back(1.0f, 0.0f);
        uvs.emplace_back(1.0f, 1.0f);
        uvs.emplace_back(0.0f, 1.0f);
    }

    std::vector<uint32_t> indices {
        0, 1, 2, // front
        2, 3, 0,
        4, 5, 6, // back
        6, 7, 4,
        8, 9, 10, // left
        10, 11, 8,
        12, 13, 14, // right
        14, 15, 12,
        16, 17, 18, // top
        18, 19, 16,
        20, 21, 22, // bottom
        22, 23, 20
    };

    return { vertices, normals, uvs, indices };
}

MeshData MeshData::Cylinder(float radius, float height, int segments) {
    std::vector<glm::vec3> vertices;
    vertices.reserve((2 + segments * 2) * 2);
    std::vector<glm::vec3> normals;
    vertices.reserve((2 + segments * 2) * 2);
    std::vector<glm::vec2> uvs;
    uvs.reserve((2 + segments * 2) * 2);

    vertices.emplace_back(0.0f, height, 0.0f); // Top = 0
    normals.emplace_back(0.0f, 1.0f, 0.0f);
    uvs.emplace_back(0.5f, 0.5f);
    vertices.emplace_back(0.0f, -height, 0.0f); // Bottom = 1
    normals.emplace_back(0.0f, -1.0f, 0.0f);
    uvs.emplace_back(0.5f, 0.5f);

    // Top & Bottom Circles
    for (int i = 1; i >= -1; i -= 2) {
        float factor = (2 * std::numbers::pi) / segments;
        for (int j = 0; j < segments; j++) {
            float x = radius * cos(factor * j);
            float z = radius * sin(factor * j);
            vertices.emplace_back(x, height * i, z);
            normals.emplace_back(0.0f, i * 1.0f, 0.0f);
            uvs.emplace_back(x * 0.5f + 0.5f, z * 0.5f + 0.5f);
        }
    }
    // Sides
    for (int i = 1; i >= -1; i -= 2) {
        float factor = (2 * std::numbers::pi) / segments;
        float uvFactor = 1.0f / segments;
        for (int j = 0; j < (segments + 1); j++) {
            glm::vec3 pos { radius * cos(factor * j), height * i, radius * sin(factor * j) };
            vertices.emplace_back(pos);
            normals.emplace_back(pos.x, 0.0f, pos.z);
            uvs.emplace_back(uvFactor * j, 0.5f + 0.5f * i);
        }
    }

    // Layout:
    // [0] top middle
    // [1] bottom middle
    // [2]-[segments+1] top circle
    // [segments+2]-[2 + segments * 2] bottom circle
    // Then again
    // top circle (first vertex is duplicated)
    // bottom circle (first vertex is duplicated)

    std::vector<uint32_t> indices;
    indices.reserve(2 * segments * 3 + segments * 6);
    int i = 2;
    for (; i < segments + 1; i++) {
        indices.emplace_back(i);
        indices.emplace_back(0);
        indices.emplace_back(i + 1);
    }
    indices.emplace_back(i);
    indices.emplace_back(0);
    indices.emplace_back(2);

    i++;
    for (; i < (1 + segments * 2); i++) {
        indices.emplace_back(1);
        indices.emplace_back(i);
        indices.emplace_back(i + 1);
    }
    indices.emplace_back(1);
    indices.emplace_back(i);
    indices.emplace_back(segments + 2);

    int offset = 2 + segments * 2;
    for (int i = 0; i < segments; i++) {
        indices.emplace_back(offset + i);
        indices.emplace_back(offset + i + 1);
        indices.emplace_back(offset + i + segments + 2); // not + 1 because the first/last of top/bottom are duplicated for uvs

        indices.emplace_back(offset + i + segments + 2);
        indices.emplace_back(offset + i + segments + 1);
        indices.emplace_back(offset + i);
    }

    return { vertices, normals, uvs, indices };
}

MeshData MeshData::Sphere(float radius, int slices, int stacks) {
    std::vector<glm::vec3> vertices;
    vertices.reserve(2 + slices * stacks);
    std::vector<glm::vec3> normals;
    vertices.reserve(2 + slices * stacks);
    std::vector<glm::vec2> uvs;
    uvs.reserve(2 + slices * stacks);

    vertices.emplace_back(0.0f, radius, 0.0f);
    normals.emplace_back(0.0f, 1.0f, 0.0f);
    uvs.emplace_back(0.0f, 0.0f);
    vertices.emplace_back(0.0f, -radius, 0.0f);
    normals.emplace_back(0.0f, -1.0f, 0.0f);
    uvs.emplace_back(1.0f, 1.0f);

    double theta = 2 * std::numbers::pi / slices;
    double phi = std::numbers::pi / (stacks + 1); // half circle only
    for (int i = 1; i < (stacks + 1); i++) {
        for (int j = 0; j < slices; j++) {
            glm::vec3 pos {
                radius * cos(theta * j) * sin(phi * i),
                radius * cos(phi * i),
                radius * sin(theta * j) * sin(phi * i)
            };
            vertices.emplace_back(pos);
            normals.emplace_back(pos);
            uvs.emplace_back(j * (1.0f / slices), i * (1.0f / (stacks)));
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

    return { vertices, normals, uvs, indices };
}




Mesh::Mesh(const MeshData& data, glm::mat4 model) {
    num_vertices = data.vertices.size();
    num_indices = data.indices.size();

    m_modelMatrix = model;

    glGenVertexArrays(1, &m_vao); 
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position attribute

    glGenBuffers(1, &m_vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * data.normals.size(), data.normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // normals

    glGenBuffers(1, &m_vboUVs);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboUVs);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * data.uvs.size(), data.uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // uvs

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * data.indices.size(), data.indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    initialized = true;
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_vboNormals);
    glDeleteBuffers(1, &m_vboUVs);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

std::shared_ptr<Mesh> Mesh::Default() {
    return std::make_shared<Mesh>(MeshData::Default(), glm::mat4(1.0f));
}

void Mesh::draw(std::shared_ptr<Shader> shader) const {
    if (!initialized) return;

    shader->use();
    shader->setUniform("model", m_modelMatrix);
    // TODO: add normal model matrix

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

