#pragma once

#include "pch.h"

class Camera {
    glm::vec3 m_pos;
    glm::vec3 m_view;
    glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projMatrix;

    // NOTE: Probably delete these when switching to quaternions
    float m_yaw = -90.0f;
    float m_pitch = 0.0f;

    const float m_speed = 3.0f;
    const float m_sensitivity = 0.2f;

public:
    Camera() = delete;
    Camera(glm::vec3 position, glm::vec3 view, float fov, float aspect, float near, float far);

    void setView(glm::vec2 mouseDelta);
    void setView(glm::vec3 newView);
    void move(glm::vec3 direction, float dt);

    glm::mat4 getViewMatrix();
    glm::mat4 getViewProjMatrix();
};
