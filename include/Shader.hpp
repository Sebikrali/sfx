#pragma once

#include "pch.h"

struct Shader {
    unsigned int m_program;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;

    void setUniform(const std::string& name, glm::vec3 vector);
    void setUniform(const std::string& name, glm::mat4 matrix);

private:
    GLint getLocation(const std::string& name);
};
