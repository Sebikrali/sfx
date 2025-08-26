#include "sfx/Texture.hpp"


Texture::Texture(const unsigned char* data, int width, int height) {
    // TODO: Add better handling (don't forget to stbi_image_free)
    assert(data);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(m_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(m_id);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(const std::string& path, bool flip) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    // TODO: Add better handling (don't forget to stbi_image_free)
    assert(data);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1, GL_RGB8, width, height);
    glTextureSubImage2D(m_id, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(m_id);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_id);
}

std::shared_ptr<Texture> Texture::Default() {
    // 2x2 checkerboard RGBA texture
    const unsigned char data[4 * 4] = {
        // Pixel order: bottom-left, bottom-right, top-left, top-right
        255,   0,   0, 255, // red
        0, 255,   0, 255,   // green
        0,   0, 255, 255,   // blue
        255, 255,   0, 255  // yellow
    };

    return std::make_shared<Texture>(data, 2, 2);
}

void Texture::use(GLuint unit) const {
    glBindTextureUnit(unit, m_id);
}
