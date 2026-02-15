#include "sfx/Color.hpp"

Color::Color(glm::vec3 color) {
    glm::vec3 c = glm::round(glm::clamp(color, .0f, 1.0f) * 255.0f);
    r = static_cast<uint8_t>(c.r);
    g = static_cast<uint8_t>(c.g);
    b = static_cast<uint8_t>(c.b);
    a = 255;
}

Color::Color(glm::vec4 color) {
    glm::vec4 c = glm::round(glm::clamp(color, .0f, 1.0f) * 255.0f);
    r = static_cast<uint8_t>(c.r);
    g = static_cast<uint8_t>(c.g);
    b = static_cast<uint8_t>(c.b);
    a = static_cast<uint8_t>(c.a);
}

Color::Color(uint32_t color) {
    r = static_cast<uint8_t>((color >> 24) & 0xFF);
    g = static_cast<uint8_t>((color >> 16) & 0xFF);
    b = static_cast<uint8_t>((color >> 8) & 0xFF);
    a = static_cast<uint8_t>(color & 0xFF);
}

Color::Color(DefaultColor color, float opacity) {
    a = glm::round(glm::clamp(opacity, .0f, 1.0f) * 255.0f);

    switch (color) {
        case WHITE:
            r = 255u;
            g = 255u;
            b = 255u;
            break;
        case RED:
            r = 255u;
            break;
        case GREEN:
            g = 255u;
        case BLUE:
            b = 255u;
            break;
        case BLACK:
            break;
    }
}


glm::vec4 Color::toFloat() const {
    return glm::clamp((glm::vec4(r, g, b, a) / 255.0f), .0f, 1.0f);
}

