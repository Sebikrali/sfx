#pragma once

#include "sfx/pch.h"

enum DefaultColor {
   WHITE = 0,
   RED,
   GREEN,
   BLUE,
   BLACK,
};

// NOTE: Maybe this could be useful
// union test {
//    struct { uint8_t r, g, b, a; };
//    uint32_t c;
// };

struct Color {
   uint8_t r{0}, g{0}, b{0}, a{0};

   Color() = default;
   explicit Color(glm::vec3 color);
   explicit Color(glm::vec4 color);
   explicit Color(uint32_t color);
   explicit Color(DefaultColor color, float opacity = 1.0f);

   glm::vec4 toFloat() const;
};

// TODO: also add a HdrColor struct that has 4 floats
