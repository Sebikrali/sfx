#pragma once

#include "pch.h"

enum CameraMode {
    FPS,
    FLY,
    ORBIT
};

class Camera {
    glm::vec3 m_pos;
    glm::vec3 m_view;
    glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
    glm::vec3 m_lookAt;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projMatrix;

    float m_yaw = -90.0f;
    float m_pitch = 0.0f;
    float m_orbitRadius = 1.0f;
    float m_maxFov;
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;
    float m_speed = 5.0f;
    const float m_sensitivity = 0.2f;

    CameraMode m_mode = FPS;

public:
    Camera() = delete;
    Camera(glm::vec3 position, glm::vec3 view, float fov, float aspect, float near, float far);

    void setMode(CameraMode mode);
    void changeSpeed(float speed);

    void setView(glm::vec2 mouseDelta);
    void setView(glm::vec3 newView);
    void setZoom(float delta);
    void move(glm::vec3 direction, float dt);
    void setPos(glm::vec3 newPos);
    void setAspectRatio(float aspectRatio);

    glm::vec3 getPos() const;
    glm::vec3 getView() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getViewProjMatrix() const;
};
