#include "sfx/Shader.hpp"

std::string loadFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        // TODO: Implement proper error handling
        throw std::runtime_error("Unable to read shader '" + path + "'");
    }

    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

void checkForErrors(unsigned int id, const std::string type) {
    int  success;
    // NOTE: This hardcoded value can also be queried with glGetShaderiv(..., GL_INFO_LOG_LENGTH, &len).
    char infoLog[1024];
    if (type != "program") {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 1024, NULL, infoLog);
            std::cout << "[Shader] Error: " << type << " shader compilation failed\n" << infoLog << "\n";
        }
    } else {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 1024, NULL, infoLog);
            std::cout << "[Shader] Error: shader program linking failed\n" << infoLog << "\n";
        }
    }
}


Shader::Shader(const std::string& vertexPathOrSrc, const std::string& fragmentPathOrSrc, bool arePaths) {
    std::string vertexShaderSource, fragmentShaderSource; 
    if (arePaths) {
        vertexShaderSource = loadFile(vertexPathOrSrc);
        fragmentShaderSource = loadFile(fragmentPathOrSrc);
    } else {
        vertexShaderSource = vertexPathOrSrc;
        fragmentShaderSource = fragmentPathOrSrc;
    }

    const char* pvertexShaderSource = vertexShaderSource.c_str();
    const char* pfragmentShaderSource = fragmentShaderSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &pvertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkForErrors(vertexShader, "vertex");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &pfragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkForErrors(fragmentShader, "fragment");

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  
    checkForErrors(m_program, "program");
}

Shader::~Shader() {
    glDeleteProgram(m_program);
}

std::shared_ptr<Shader> Shader::Default() {
    const std::string vertexShaderSrc = R"(#version 450 core

        layout(location = 0) in vec3 pos;
        layout(location = 1) in vec3 normal;
        layout(location = 2) in vec2 uv;

        uniform mat4 model;
        uniform mat4 viewProj;

        out vec3 fragPos;
        out vec3 fragNormal;
        out vec2 fragUV;

        void main() {
        gl_Position = viewProj * model * vec4(pos, 1.0);
        fragPos = vec3(model * vec4(pos, 1.0));
        fragNormal = normal;
        fragUV = uv;
        }
    )";
    const std::string fragmentShaderSrc = R"(
        #version 450 core

        in vec3 fragPos;
        in vec3 fragNormal;
        in vec2 fragUV;

        uniform vec3 viewPos;

        struct PointLight {
        vec3 pos;
        vec3 color;
        vec3 attenuation;
        };
        uniform PointLight pointLight;

        struct Material {
        vec4 coefficients;
        vec3 color;
        };
        uniform Material material;

        uniform sampler2D Texture;

        uniform vec4 lightMode;
        uniform vec3 drawNormalsUVs;
        uniform vec2 hideMaterialTexture;

        out vec4 color;

        void main() {
        if (drawNormalsUVs.x == 1.0) {
        color = vec4(normalize(fragNormal), 1.0);
        return;
        } else if (drawNormalsUVs.y == 1.0) {
        color = vec4(fragUV, 0.0, 1.0);
        return;
        }

        float mixFactor = 0.5;
        vec4 matCoeffs = material.coefficients;
        vec3 matColor = material.color;
        if (hideMaterialTexture.x == 1.0) {
        matCoeffs = vec4(1.0, 1.0, 0.0, 0.0);
        matColor = vec3(1.0);
        mixFactor = 1.0;
        }

        vec3 texColor = texture(Texture, fragUV).xyz;
        if (hideMaterialTexture.y == 1.0) {
        texColor = vec3(1.0);
        mixFactor = 0.0;
        }

        vec3 fragColor = mix(matColor, texColor, mixFactor);

        if (lightMode.w == 0.0) {
        color = vec4(fragColor, 1.0);
        return;
        }

        vec3 norm = normalize(fragNormal);
        vec3 lightDir = pointLight.pos - fragPos;
        float dist = length(lightDir);
        float attenuation = 1.0 / (pointLight.attenuation.x + pointLight.attenuation.y * dist + pointLight.attenuation.z * (dist * dist));
        lightDir = normalize(lightDir);

        vec3 ambient = pointLight.color * matCoeffs.x * attenuation;

        vec3 diffuse = pointLight.color * (matCoeffs.y * max(dot(lightDir, norm), 0.0)) * attenuation;

        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), matCoeffs.w);
        vec3 specular = pointLight.color * matCoeffs.z * spec * attenuation;

        color = vec4((lightMode.x * ambient + lightMode.y * diffuse + lightMode.z * specular) * fragColor, 1.0);
        }
    )";

    return std::make_shared<Shader>(vertexShaderSrc, fragmentShaderSrc, false);
}

std::shared_ptr<Shader> Shader::TextShader() {
    const std::string vertexShaderSrc = R"(
        #version 450 core

        layout(location = 0) in vec2 pos;
        layout(location = 2) in vec2 uv;

        uniform mat4 model;
        uniform mat4 viewProj;

        out vec2 fragUV;

        void main() {
        gl_Position = viewProj * model * vec4(pos, 0.0, 1.0);
        fragUV = uv;
        })";
    const std::string fragmentShaderSrc = R"(
        #version 450 core

        in vec2 fragUV;

        uniform sampler2D Texture;

        out vec4 color;

        void main() {
        color = vec4(0.0, 0.0, 0.0, texture(Texture, fragUV).r);
        })";

    return std::make_shared<Shader>(vertexShaderSrc, fragmentShaderSrc, false);
}

void Shader::use() const {
    glUseProgram(m_program);
}

void Shader::setUniform(const std::string& name, bool value) {
    glUniform1i(getLocation(name), value);
}

void Shader::setUniform(const std::string& name, float value) {
    glUniform1f(getLocation(name), value);
}

void Shader::setUniform(const std::string& name, glm::vec2 vector) {
    glUniform2fv(getLocation(name), 1, glm::value_ptr(vector));
}

void Shader::setUniform(const std::string& name, glm::vec3 vector) {
    glUniform3fv(getLocation(name), 1, glm::value_ptr(vector));
}

void Shader::setUniform(const std::string& name, glm::vec4 vector) {
    glUniform4fv(getLocation(name), 1, glm::value_ptr(vector));
}

void Shader::setUniform(const std::string& name, glm::mat4 matrix) {
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(const std::string& name, const PointLight& light) {
    glUniform3fv(getLocation(name + ".pos"), 1, glm::value_ptr(light.pos));
    glUniform3fv(getLocation(name + ".color"), 1, glm::value_ptr(light.color));
    glUniform3fv(getLocation(name + ".attenuation"), 1, glm::value_ptr(light.attenuation));
}

void Shader::setUniform(const std::string& name, const DirLight& light) {
    glUniform3fv(getLocation(name + ".direction"), 1, glm::value_ptr(light.direction));
    glUniform3fv(getLocation(name + ".color"), 1, glm::value_ptr(light.color));
}

void Shader::setUniform(const std::string& name, const SpotLight& light) {
    glUniform3fv(getLocation(name + ".pos"), 1, glm::value_ptr(light.pos));
    glUniform3fv(getLocation(name + ".direction"), 1, glm::value_ptr(light.direction));
    glUniform1f(getLocation(name + ".cutoff"), glm::cos(light.cutoff));
    glUniform3fv(getLocation(name + ".color"), 1, glm::value_ptr(light.color));
}


GLint Shader::getLocation(const std::string& name) {
    return glGetUniformLocation(m_program, name.c_str());
}

