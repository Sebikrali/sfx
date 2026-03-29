#pragma once

#include "pch.h"

struct ScreenQuad {
    unsigned int vao;
    unsigned int vbo, ebo;

    ScreenQuad();
    ScreenQuad(glm::vec4 bounds);

    void draw() const;
};

