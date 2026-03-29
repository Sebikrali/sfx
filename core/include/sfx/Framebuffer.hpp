#pragma once

#include "pch.h"

struct Framebuffer {
    unsigned int fbo;
    unsigned int depthAttachment;

    /**
     * @brief Creates a OpenGL framebuffer and binds it.
     */
    Framebuffer();
    ~Framebuffer();
};

