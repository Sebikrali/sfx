#include "DebugUtils.hpp"

void Debug::printVec3(glm::vec3 vec, const std::string& name) {
    std::cout << std::format("{}({:.2},{:.2},{:.2})\n", name, vec.x, vec.y, vec.z);
}

std::string Debug::toString(glm::vec3 vec) {
    std::ostringstream oss;
    oss << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
    return oss.str();
}

std::string Debug::toString(glm::mat4 mat) {
    std::ostringstream oss;
    oss << "|" << mat[0][0] << ", " << mat[1][0] << ", " << mat[2][0] << ", " << mat[3][0] << "|\n";
    oss << "|" << mat[0][1] << ", " << mat[1][1] << ", " << mat[2][1] << ", " << mat[3][1] << "|\n";
    oss << "|" << mat[0][2] << ", " << mat[1][2] << ", " << mat[2][2] << ", " << mat[3][2] << "|\n";
    oss << "|" << mat[0][3] << ", " << mat[1][3] << ", " << mat[2][3] << ", " << mat[3][3] << "|\n";
    return oss.str();
}

