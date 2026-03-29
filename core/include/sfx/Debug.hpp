#pragma once

#include "pch.h"

#include <source_location>

#ifdef NDEBUG 
    #define ASSERT(cond, msg) ((void)0)
#else
    #define ASSERT(cond, msg) ::assert_msg((cond), (msg))
#endif

inline void assert_msg(bool cond, const char* msg, const std::source_location& loc = std::source_location::current()) {
    if (!cond) {
        std::cerr << "Assertion failed: " << msg << '\n'
                  << "File: " << loc.file_name()
                  << ", line: " << loc.line()
                  << ", function: " << loc.function_name() << '\n';
        std::abort();
    }
}

void error_and_exit(const std::string& message);

std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
void APIENTRY DebugCallbackGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const GLvoid *userParam);
