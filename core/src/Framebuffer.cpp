#include "sfx/Framebuffer.hpp"

Framebuffer::Framebuffer() {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &fbo);  
}

