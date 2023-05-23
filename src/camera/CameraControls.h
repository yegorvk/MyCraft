//
// Created by egorv on 4/9/2023.
//

#ifndef SHITCRAFT_CAMERACONTROLS_H
#define SHITCRAFT_CAMERACONTROLS_H

#include <functional>

#include "Camera.h"
#include "../core/EventConsumer.h"
#include "../core/UpdateDelegate.h"

struct CameraControlBindings {
    SDL_Scancode forward = SDL_SCANCODE_W, back = SDL_SCANCODE_S;
    SDL_Scancode left = SDL_SCANCODE_A, right = SDL_SCANCODE_D;
    SDL_Scancode up = SDL_SCANCODE_SPACE, down = SDL_SCANCODE_LSHIFT;
    SDL_Scancode rollLeft = SDL_SCANCODE_Q, rollRight = SDL_SCANCODE_E;
};

constexpr float DEFAULT_CAMERA_SENSITIVITY = 0.0005f;
constexpr float DEFAULT_CAMERA_SENSITIVITY_KEYBOARD_COEFFICIENT = 1.f;
constexpr float DEFAULT_CAMERA_SPEED = 0.005f;

struct PressedControlKeys {
    PressedControlKeys() {
        forward = back = left = right = up = down = rollLeft = rollRight = false;
    }

    bool forward: 1;
    bool back: 1;
    bool left: 1;
    bool right: 1;
    bool up: 1;
    bool down: 1;
    bool rollLeft: 1;
    bool rollRight: 1;
};

class CameraControls : public EventConsumer, public UpdateDelegate {
public:
    inline explicit CameraControls(Camera &camera,
                                   float speed = DEFAULT_CAMERA_SPEED,
                                   float sensitivity = DEFAULT_CAMERA_SENSITIVITY,
                                   CameraControlBindings bindings = CameraControlBindings())
            : camera(camera), bindings(bindings), sensitivity(sensitivity), speed(speed) {};

    ~CameraControls() override = default;

    bool handleEvent(const SDL_Event &event) final;

    void update(uint64_t deltaMs) final;

private:
    Camera &camera;
    CameraControlBindings bindings;
    float sensitivity, speed;

    PressedControlKeys controlKeys;
};

#endif //SHITCRAFT_CAMERACONTROLS_H
