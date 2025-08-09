#include "Text.hpp"

Text::Text() {
    model = glm::mat4(1.0f);
}

Text::Text(const std::string& value, const std::vector<FontChar>& characters, glm::mat4 model) {
    this->value = value;
    this->model = model;

    projection = glm::ortho(0.0f, 1280.0f, 0.0f, 768.0f);

    float x = 0.0f;
    for (const auto& ch : characters) {
        float xpos = x + ch.bearing.x;
        float ypos = 0.0f - (ch.size.y - ch.bearing.y);

        float w = ch.size.x;
        float h = ch.size.y;

        // FreeType texture space is (0,0) = top left
        float vertices[6][4] = {
            { xpos,     ypos,       0.0f, 1.0f }, // bottom left
            { xpos + w, ypos,       1.0f, 1.0f }, // bottom right
            { xpos + w, ypos + h,   1.0f, 0.0f }, // top right

            { xpos + w, ypos + h,   1.0f, 0.0f }, // top right
            { xpos,     ypos + h,   0.0f, 0.0f }, // top left
            { xpos,     ypos,       0.0f, 1.0f }  // bottom left
        };
        x += ch.advance >> 6;

        GLuint vao;
        glCreateVertexArrays(1, &vao);

        GLuint vbo;
        glCreateBuffers(1, &vbo);
        glNamedBufferData(vbo, sizeof(float) * 6 * 4, vertices, GL_STATIC_DRAW);
        glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 4);

        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribBinding(vao, 0, 0);
        glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(vao, 2);
        glVertexArrayAttribBinding(vao, 2, 0);
        glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2);

        chars.emplace_back(ch);
        quads.emplace_back(std::array<GLuint, 2>{vao, vbo});
    }
}

Text::~Text() {
    for (const auto& ch : chars) {
        glDeleteTextures(1, &ch.texture);
    }

    for (const auto& quad : quads) {
        glDeleteBuffers(1, &quad[1]);
        glDeleteVertexArrays(1, &quad[0]);
    }
}

void Text::draw() const {
    size_t i = 0;
    for (const auto& quad : quads) {
        glBindTextureUnit(0, chars[i].texture);
        glBindVertexArray(quad[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        i++;
    }
}

void Text::draw(std::shared_ptr<Shader> shader) const {
    shader->use();
    shader->setUniform("viewProj", projection);
    shader->setUniform("model", glm::mat4(1.0f));

    size_t i = 0;
    for (const auto& quad : quads) {
        glBindTextureUnit(0, chars[i].texture);
        glBindVertexArray(quad[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        i++;
    }
}

TextCollection::TextCollection(std::shared_ptr<std::unordered_map<char, FontChar>> characters) {
    this->characters = characters;
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 4);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(vao, 2);
    glVertexArrayAttribBinding(vao, 2, 0);
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2);
}

TextCollection::~TextCollection() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void TextCollection::drawDynamicText(DynamicText text) const {
    glBindVertexArray(vao);
    float x = 0.0f;
    for (const auto& c : text.value) {
        if (!characters->contains(c)) {
            // TODO: Find a better way of handling this, maybe a error character
            throw std::runtime_error(std::format("Char {}/{} not loaded", c, std::to_string(c)));
        }
        auto& ch = characters->at(c);

        float xpos = x + ch.bearing.x;
        float ypos = 0.0f - (ch.size.y - ch.bearing.y);
        float w = ch.size.x;
        float h = ch.size.y;

        // FreeType texture space is (0,0) = top left
        float vertices[6][4] = {
            { xpos,     ypos,       0.0f, 1.0f }, // bottom left
            { xpos + w, ypos,       1.0f, 1.0f }, // bottom right
            { xpos + w, ypos + h,   1.0f, 0.0f }, // top right

            { xpos + w, ypos + h,   1.0f, 0.0f }, // top right
            { xpos,     ypos + h,   0.0f, 0.0f }, // top left
            { xpos,     ypos,       0.0f, 1.0f }  // bottom left
        };
        x += ch.advance >> 6;

        glNamedBufferSubData(vbo, 0, sizeof(float) * 6 * 4, vertices);
        glBindTextureUnit(0, ch.texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
}

void TextCollection::draw() const {
    shader->use();
    shader->setUniform("viewProj", projection);
    for (const auto& [_, text] : texts) {
        shader->setUniform("model", text->model);
        text->draw();
    }

    for (const auto& [_, text] : dynamicTexts) {
        shader->setUniform("model", text.model);
        drawDynamicText(text);
    }
}


FontManager::FontManager() {
    if (FT_Init_FreeType(&m_ft)) {
        throw std::runtime_error("Couldn't init FreeType");
    }

    if (FT_New_Face(m_ft, "assets/fonts/JetBrainsMono-Medium.ttf", 0, &m_face)) {
        throw std::runtime_error("Couldn't load the font");
    }
    FT_Set_Pixel_Sizes(m_face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (uint8_t c = 0; c < 128; c++) {
        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) {
            throw std::runtime_error(std::format("Couldn't load char {}", c));
        }

        if (m_face->glyph->bitmap.width == 0 || m_face->glyph->bitmap.rows == 0) {
            characters.emplace(c, FontChar(
                0,
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                m_face->glyph->advance.x
            ));
        } else {
            GLuint texture;
            glCreateTextures(GL_TEXTURE_2D, 1, &texture);
            glTextureStorage2D(texture, 1, GL_R8, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows);
            glTextureSubImage2D(texture, 0, 0, 0, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);

            glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            characters.emplace(c, FontChar(
                texture,
                glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
                glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
                m_face->glyph->advance.x
            ));
        }
    }
}

FontManager::~FontManager() {
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_ft);
}


Text FontManager::createText(const std::string& value, glm::vec2 pos) {
    std::vector<FontChar> chars;
    for (const auto& c : value) {
        if (!characters.contains(c)) {
            throw std::runtime_error(std::format("Char {}/{} not loaded", c, std::to_string(c)));
        }
        chars.emplace_back(characters[c]);
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));

    return Text(value, chars, model);
}

TextCollection FontManager::createTextCollection() {
    return TextCollection(std::make_shared<std::unordered_map<char, FontChar>>(characters));
}
