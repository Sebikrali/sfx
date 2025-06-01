#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 view, float fov, float aspect, float near, float far) : m_pos(position), m_view(view) {
    m_projMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::setView(glm::vec2 mouseDelta) {
    m_yaw += mouseDelta.x * m_sensitivity;
    m_pitch -= mouseDelta.y * m_sensitivity;

    if (m_pitch > 89.0f) {
        m_pitch = 89.0f;
    } else if (m_pitch < -89.0f) {
        m_pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_view = glm::normalize(direction);
}

void Camera::setView(glm::vec3 newView) {
    m_view = newView;
}

void Camera::move(glm::vec3 direction, float dt) {
    glm::vec3 right = glm::normalize(glm::cross(m_view, m_up));
    glm::vec3 view = glm::vec3(m_view.x, 0.0f, m_view.z);
    m_pos += glm::normalize(right * direction.x +  view * -direction.z) * m_speed * dt;
}

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 right = glm::normalize(glm::cross(m_view, m_up));
    glm::vec3 up = glm::normalize(glm::cross(right, m_view));
    glm::vec3 view = -glm::normalize(m_view);
    glm::mat4 R = {
        {right.x, up.x, view.x, 0.0f},
        {right.y, up.y, view.y, 0.0f},
        {right.z, up.z, view.z, 0.0f},
        {0.0f,    0.0f, 0.0f,   1.0f}
    }; 
    glm::mat4 T = glm::translate(glm::mat4(1.0f), -m_pos);
    return R * T;
}

glm::mat4 Camera::getViewProjMatrix() {
    return m_projMatrix * getViewMatrix();
}

