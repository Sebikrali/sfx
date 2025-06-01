#include "Debug.hpp"

void error_and_exit(const std::string& message) {
    std::cout << "[Main] Error: " << message << std::endl;
    exit(EXIT_FAILURE);
}

void APIENTRY DebugCallbackGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const GLvoid *userParam) {
    std::string error = FormatDebugOutput(source, type, id, severity, message);
    std::cout << error << std::endl;
}

std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
    std::string sourceString;
    switch (source) {
        case GL_DEBUG_SOURCE_API: 
            sourceString = "API";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            sourceString = "Application";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceString = "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceString = "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceString = "Third Party";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            sourceString = "Other";
            break;
        default:
            sourceString = "Unknown";
            break;
    }

    std::string typeString;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            typeString = "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeString = "Deprecated Behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeString = "Undefined Behavior";
            break;
            typeString = "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            typeString = "Performance";
            break;
        case GL_DEBUG_TYPE_OTHER:
            typeString = "Other";
            break;
        default:
            typeString = "Unknown";
            break;
    }

    std::string severityString;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            severityString = "High";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severityString = "Medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            severityString = "Low";
            break;
        default:
            severityString = "Unknown";
            break;
    }

    std::stringstream stringStream;
    stringStream << "OpenGL Error: " << msg;
    stringStream << " [Source = " << sourceString;
    stringStream << ", Type = " << typeString;
    stringStream << ", Severity = " << severityString;
    stringStream << ", ID = " << id << "]";
    return stringStream.str();
}


