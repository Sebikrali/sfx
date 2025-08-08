#pragma once

#include "pch.h"

#include "Shader.hpp"

struct FontChar {
    GLuint texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    uint32_t advance;
};

struct Text {
    std::string value;

    glm::mat4 projection;
    glm::mat4 model;

    Text(const std::string& value, const std::vector<FontChar>& characters, glm::mat4 model); 
    ~Text();

    void draw(std::shared_ptr<Shader> shader) const;

private:
    std::vector<FontChar> chars;
    std::vector<std::array<GLuint, 2>> quads;
};


class FontManager {
    FT_Library m_ft;
    FT_Face m_face;
    std::unordered_map<char, FontChar> characters;

public:
    FontManager();
    ~FontManager();

    void setFont(const std::string& filePath, uint32_t pixelSizes);

    Text createText(const std::string& value, glm::vec2 pos);
};

