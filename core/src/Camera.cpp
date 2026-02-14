#include "sfx/Camera.hpp"
#include "glm/geometric.hpp"

Camera::Camera(glm::vec3 position, float fov, float aspect, float near, float far) : 
    m_pos(position), 
    m_projMatrix(glm::perspective(glm::radians(fov), aspect, near, far)),
    m_orientation(1.0f, 0.0f, 0.0f, 0.0f),
    m_maxFov(fov),
    m_fov(fov),
    m_aspect(aspect),
    m_near(near),
    m_far(far) {}

Camera::Camera(glm::vec3 position, glm::vec3 view, float fov, float aspect, float near, float far) : 
    m_pos(position), 
    m_projMatrix(glm::perspective(glm::radians(fov), aspect, near, far)),
    m_maxFov(fov),
    m_fov(fov),
    m_aspect(aspect),
    m_near(near),
    m_far(far) {
    m_orientation = glm::angleAxis(0.0f, view);
}


void Camera::updateView(glm::vec2 mouseDelta) {
    float sens = m_sensitivity;
    if (m_mode != ORBIT) {
        sens = m_sensitivity * (m_fov / m_maxFov);
    }

    m_yaw -= mouseDelta.x * sens;
    m_pitch = glm::clamp(m_pitch - mouseDelta.y * sens, -89.0f, 89.0f);

    glm::quat yawQuat = glm::angleAxis(glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat pitchQuat = glm::angleAxis(glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));

    m_orientation = glm::normalize(yawQuat * pitchQuat);

    if (m_mode == ORBIT) {
        m_pos = m_lookAt + m_orbitRadius * -getView();
    }
}

void Camera::move(glm::vec3 direction, float dt) {
    glm::vec3 movement = m_orientation * direction;
    if (m_mode != FLY) {
        movement.y = 0.0f;
    }
    movement = glm::normalize(movement) * m_speed * dt;
    m_pos += movement;
    m_lookAt += movement;
}

void Camera::zoom(float delta) {
    if (m_mode == ORBIT) {
        m_orbitRadius = glm::max(m_orbitRadius + delta * m_sensitivity, 0.1f);
        m_pos = m_lookAt + (m_orbitRadius * -getView());
    } else {
        m_fov = glm::clamp(m_fov + delta, 1.0f, m_maxFov);
        m_projMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }
}


void Camera::setMode(CameraMode mode) {
    m_mode = mode;

    if (mode == ORBIT) {
        m_lookAt = m_pos + getView();
        m_orbitRadius = 1.0f;
    }
}

void Camera::setPos(glm::vec3 newPos) {
    m_pos = newPos;
    m_lookAt = m_pos + getView();
}

void Camera::setView(glm::vec3 newView) {
    m_orientation = glm::angleAxis(0.0f, newView);
}

void Camera::setSpeed(float speed) {
    m_speed = speed;
}

void Camera::setAspectRatio(float aspectRatio) {
    m_projMatrix = glm::perspective(glm::radians(m_fov), aspectRatio, m_near, m_far);
}


glm::vec3 Camera::getPos() const {
    return m_pos;
}

glm::vec3 Camera::getView() const {
    return glm::normalize(m_orientation * glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::mat4 Camera::getViewMatrix() const {
    glm::vec3 view = -getView();
    glm::vec3 right = glm::normalize(m_orientation * glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 up = glm::normalize(m_orientation * glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 R = {
        {right.x, up.x, view.x, 0.0f},
        {right.y, up.y, view.y, 0.0f},
        {right.z, up.z, view.z, 0.0f},
        {0.0f,    0.0f, 0.0f,   1.0f}
    }; 
    glm::mat4 T = glm::translate(glm::mat4(1.0f), -m_pos);
    return R * T;
}

glm::mat4 Camera::getViewProjMatrix() const {
    return m_projMatrix * getViewMatrix();
}

