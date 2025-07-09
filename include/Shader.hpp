#pragma once

#include "Light.hpp"
#include "pch.h"

struct Shader {
    unsigned int m_program;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;

    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, glm::vec3 vector);
    void setUniform(const std::string& name, glm::mat4 matrix);
    void setUniform(const std::string& name, const PointLight& light);

private:
    GLint getLocation(const std::string& name);
};
