#pragma once

#include "pch.h"

enum CameraMode {
    FLY,
    FPS,
    ORBIT
};

class Camera {
public:
    Camera() = delete;
    Camera(glm::vec3 position, float fov, float aspect, float near, float far);
    Camera(glm::vec3 position, glm::vec3 view, float fov, float aspect, float near, float far);

    void updateView(glm::vec2 mouseDelta);
    void move(glm::vec3 direction, float dt);
    void zoom(float delta);

    void setMode(CameraMode mode);
    void setPos(glm::vec3 newPos);
    void setView(glm::vec3 newView);
    void setSpeed(float speed);
    void setAspectRatio(float aspectRatio);

    glm::vec3 getPos() const;
    glm::vec3 getView() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getViewProjMatrix() const;

private:
    glm::vec3 m_pos;
    glm::vec3 m_lookAt;
    glm::quat m_orientation;

    float m_yaw = -0.0f;
    float m_pitch = 0.0f;
    float m_orbitRadius = 1.0f;

    glm::mat4 m_projMatrix;
    float m_maxFov;
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;

    CameraMode m_mode = FLY;
    float m_speed = 5.0f;
    const float m_sensitivity = 0.2f;
};
