#pragma once

#include "pch.h"

namespace Debug {
    void printVec3(glm::vec3 vec, const std::string& name = "vec");

    std::string toString(glm::vec3 vec);
    std::string toString(glm::mat4 mat);
}

