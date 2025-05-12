#include "Shader.hpp"

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


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexShaderSource = loadFile(vertexPath);
    const char* pvertexShaderSource = vertexShaderSource.c_str();

    std::string fragmentShaderSource = loadFile(fragmentPath);
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

void Shader::use() const {
    glUseProgram(m_program);
}

void Shader::setUniform(const std::string& name, glm::vec3 vector) {
    glUniform3fv(getLocation(name), 1, glm::value_ptr(vector));
}

void Shader::setUniform(const std::string& name, glm::mat4 matrix) {
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}


GLint Shader::getLocation(const std::string& name) {
    return glGetUniformLocation(m_program, name.c_str());
}

