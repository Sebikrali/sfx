#pragma once

#include "pch.h"

// NOTE: Maybe change to struct, if this class doesn't have much implementation
struct Texture {
    Texture() = delete;
    Texture(const unsigned char* data, int width, int height);
    Texture(const std::string& path, bool flip = false); // NOTE: Maybe change this default value, depending on field experience
    ~Texture();

    static std::shared_ptr<Texture> Default();

    void use(GLuint unit = 0) const;

private:
    GLuint m_id;
};
