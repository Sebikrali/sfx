#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 view, float fov, float aspect, float near, float far) : m_pos(position), m_view(view) {
    m_projMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::move(Direction dir, float dt) {
    glm::vec3 direction;
    switch (dir) {
        case Direction::Forward:
            direction = m_view;
            break;
        case Direction::Backward:
            direction = -m_view;
            break;
        case Direction::Left:
            direction = glm::cross(m_up, m_view);
            break;
        case Direction::Right:
            direction = glm::cross(m_view, m_up);
            break;
    }

    m_pos += glm::normalize(direction) * m_speed * dt;
}

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 right = glm::normalize(glm::cross(m_view, m_up));
    glm::vec3 up = glm::normalize(m_up);
    glm::vec3 view = -glm::normalize(m_view);
    glm::mat4 R = { // Rotation
        {right.x, right.y, right.z, 0.0f},
        {up.x,    up.y,    up.z,    0.0f},
        {view.x,  view.y,  view.z,  0.0f},
        {0.0f,    0.0f,    0.0f,    1.0f}
    }; 
    glm::mat4 T = glm::translate(glm::mat4(1.0f), -m_pos); // Translation
    return T * R;

    // NOTE: This always looks at (0,0,0) so only the right version for some types of cameras
    // return glm::lookAt(m_pos, {0.0f, 0.0f, 0.0f}, m_up);
}

glm::mat4 Camera::getViewProjMatrix() {
    return m_projMatrix * getViewMatrix();
}

