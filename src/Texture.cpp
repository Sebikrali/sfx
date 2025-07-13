#include "Texture.hpp"

Texture::Texture(const std::string& path, bool flip) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    // TODO: Add better handling (don't forget to stbi_image_free)
    assert(data);

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_id);
}

std::shared_ptr<Texture> Texture::Default() {
    return std::make_shared<Texture>("assets/textures/default.png");
}

void Texture::use(GLuint unit) const {
    glBindTextureUnit(unit, m_id);
}
