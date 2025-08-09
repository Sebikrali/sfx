#pragma once

#include "pch.h"

#include "Shader.hpp"

struct FontChar {
    GLuint texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    uint32_t advance;
};

// TODO: Rewrite this after finding the right abstraction
struct Text {
    std::string value;

    glm::mat4 projection;
    glm::mat4 model;

    Text();
    Text(const std::string& value, const std::vector<FontChar>& characters, glm::mat4 model); 
    ~Text();

    void draw() const;
    void draw(std::shared_ptr<Shader> shader) const;

private:
    std::vector<FontChar> chars;
    std::vector<std::array<GLuint, 2>> quads;
};

struct DynamicText {
    std::string value;
    glm::mat4 model = glm::mat4(1.0f);
};

struct TextCollection {
    std::unordered_map<std::string, std::shared_ptr<Text>> texts;
    std::unordered_map<std::string, DynamicText> dynamicTexts;
    std::shared_ptr<Shader> shader;
    glm::mat4 projection;

    TextCollection() = delete;
    TextCollection(std::shared_ptr<std::unordered_map<char, FontChar>> characters);
    ~TextCollection();

    void draw() const;

private:
    std::shared_ptr<std::unordered_map<char, FontChar>> characters;
    GLuint vao, vbo;

    void drawDynamicText(DynamicText text) const;
};


class FontManager {
    FT_Library m_ft;
    FT_Face m_face;
    std::unordered_map<char, FontChar> characters;

public:
    uint32_t fontSize = 24;

    FontManager();
    ~FontManager();

    void setFont(const std::string& filePath, uint32_t pixelSizes);

    Text createText(const std::string& value, glm::vec2 pos);
    TextCollection createTextCollection();
};

