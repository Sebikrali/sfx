#pragma once

#include "pch.h"

// NOTE: Maybe change to struct, if this class doesn't have much implementation
class Texture {
    GLuint m_id;

public: 
    Texture() = delete;
    Texture(const std::string& path, bool flip = false); // NOTE: Maybe change this default value, depending on field experience
    ~Texture();

    void draw(GLuint unit = 0) const;
};
