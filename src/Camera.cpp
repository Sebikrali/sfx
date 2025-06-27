#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 view, float fov, float aspect, float near, float far) : 
    m_pos(position), 
    m_view(view), 
    m_projMatrix(glm::perspective(glm::radians(fov), aspect, near, far)),
    m_maxFov(fov),
    m_fov(fov),
    m_aspect(aspect),
    m_near(near),
    m_far(far) {}

void Camera::setMode(CameraMode mode) {
    CameraMode last = m_mode;
    m_mode = mode;

    // TODO: if ORBIT change the values of member variables
    if (mode == ORBIT) {
        m_lookAt = m_pos + m_view;
        m_orbitRadius = 1.0f;
        // setView({0.0f, 0.0f});
    }

    if (last == ORBIT) {
        m_yaw *= -1.0f;
        m_pitch *= -1.0f;
    }
}

void Camera::changeSpeed(float speed) {
    m_speed = speed;
}

void Camera::setView(glm::vec2 mouseDelta) {
    float sens = m_sensitivity;
    if (m_mode != ORBIT) {
        sens = m_sensitivity * (m_fov / m_maxFov);
    }

    m_yaw += mouseDelta.x * sens;
    m_pitch -= mouseDelta.y * sens;

    if (m_pitch > 89.0f) {
        m_pitch = 89.0f;
    } else if (m_pitch < -89.0f) {
        m_pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    if (m_mode == ORBIT) {
        m_view = glm::normalize(m_lookAt - m_pos);
        m_pos = m_lookAt + m_orbitRadius * direction;
    } else {
        m_view = direction;
    }

    // std::cout << std::format("pos({:.2f},{:.2f},{:.2f}), view({:.2f},{:.2f},{:.2f})\n", m_pos.x, m_pos.y, m_pos.z, m_view.x, m_view.y, m_view.z);
}

void Camera::setView(glm::vec3 newView) {
    if (m_mode == ORBIT) return; // NOTE: May not need this, should be fine for orbit mode to set view directly
    m_view = newView;
}

void Camera::setZoom(float delta) {
    if (m_mode == ORBIT) {
        m_orbitRadius += delta * m_sensitivity;
        if (m_orbitRadius < 0.1) {
            m_orbitRadius = 0.1;
        }
        m_pos = m_lookAt + (m_orbitRadius * -m_view);
    } else {
        m_fov += delta;
        if (m_fov < 1.0f) {
            m_fov = 1.0f;
        } else if (m_fov > m_maxFov) {
            m_fov = m_maxFov;
        }
        m_projMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }
}

void Camera::move(glm::vec3 direction, float dt) {
    float y = 0.0f;
    if (m_mode == FLY) {
        y = m_view.y;
    }
    glm::vec3 right = glm::normalize(glm::cross(m_view, m_up));
    glm::vec3 view = glm::vec3(m_view.x, y, m_view.z);
    glm::vec3 movement = glm::normalize(right * direction.x + view * -direction.z) * m_speed * dt;
    m_pos += movement;
    m_lookAt += movement;
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

