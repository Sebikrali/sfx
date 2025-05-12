#pragma once

#include "pch.h"

enum class Direction {
    Forward,
    Backward,
    Left,
    Right
};

class Camera {
    glm::vec3 m_pos;
    glm::vec3 m_view;
    glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projMatrix;

    const float m_speed = 3.0f;

public:
    Camera() = delete;
    Camera(glm::vec3 position, glm::vec3 view, float fov, float aspect, float near, float far);

    void setView(glm::vec2 mouseDelta, float timeDelta);
    void setView(glm::vec3 newView);

    /*!
     * Moves the camera along the view direction (Forwards, Backwards, Left, Right)
     */
    void move(Direction dir, float dt);

    glm::mat4 getViewMatrix();
    glm::mat4 getViewProjMatrix();
};
