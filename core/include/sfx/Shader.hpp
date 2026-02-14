#pragma once

#include "pch.h"

#include "Light.hpp"

enum ShaderResourceBits {
    DYNAMIC_LIGHTS = 0x00000001,
};

enum ShaderType {
    VERT,
    FRAG,
    // Add others when needed
};

struct ShaderDefine {
    ShaderType type;
    std::string_view name;
    int32_t value;
};

struct Shader {
    ShaderResourceBits resources;

    Shader() = delete;
    Shader(const std::string& vertexPathOrSrc, const std::string& fragmentPathOrSrc, const std::vector<ShaderDefine>& defines = {}, bool arePaths = true);
    ~Shader();

    static std::shared_ptr<Shader> Default();
    static std::shared_ptr<Shader> TextShader();

    void use() const;

    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, glm::vec2 vector);
    void setUniform(const std::string& name, glm::vec3 vector);
    void setUniform(const std::string& name, glm::vec4 vector);
    void setUniform(const std::string& name, glm::mat3 matrix);
    void setUniform(const std::string& name, glm::mat4 matrix);
    void setUniform(const std::string& name, const PointLight& light);
    void setUniform(const std::string& name, const DirLight& light);
    void setUniform(const std::string& name, const SpotLight& light);

private:
    unsigned int m_program;

    GLint getLocation(const std::string& name);
};
