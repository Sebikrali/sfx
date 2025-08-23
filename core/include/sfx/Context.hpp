#pragma once

#include "Camera.hpp"

enum CullMode {
    BACK,
    FRONT,
    OFF
};

struct RenderContext {
    bool drawDebugHud = false;
    bool drawWireframe = false;
    CullMode cullMode = BACK;
    glm::vec3 lightMode = { 1.0f, 1.0f, 1.0f };
    glm::vec3 drawNormalsUVs = { 0.0f, 0.0f, 0.0f };

    Camera camera;
    CameraMode cameraMode = FPS;

    RenderContext(Camera camera) : camera(camera) {}
};

struct WindowContext {
    double xPos = 0.0;
    double yPos = 0.0;

    // Store width and height only if window was manually resized, to go back to this after fullscreen
    int width = -1;
    int height = -1;
    bool fullscreen = false;
    int fullscreenWidth = 0;
    int fullscreenHeight = 0;

    bool mouseCaptured = true;
    bool dragging = false;
    bool firstMouse = true;
};
