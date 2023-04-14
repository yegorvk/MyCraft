//
// Created by egorv on 4/9/2023.
//

#include "CameraControls.h"
#include "spdlog/spdlog.h"

bool CameraControls::handleEvent(const SDL_Event &event) {
    if (event.type == SDL_MOUSEMOTION) {
        int xRel = event.motion.xrel, yRel = event.motion.yrel;
        camera.rotate(sensitivity * glm::vec3(static_cast<float>(yRel), static_cast<float>(xRel), 0.f));
    } else if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) {
        bool down = event.type == SDL_KEYDOWN;
        SDL_Scancode scancode = event.key.keysym.scancode;

        if (scancode == bindings.forward)
            controlKeys.forward = down;
        else if (scancode == bindings.back)
            controlKeys.back = down;
        else if (scancode == bindings.left)
            controlKeys.left = down;
        else if (scancode == bindings.right)
            controlKeys.right = down;
        else if (scancode == bindings.up)
            controlKeys.up = down;
        else if (scancode == bindings.down)
            controlKeys.down = down;
        else if (scancode == bindings.rollLeft)
            controlKeys.rollLeft = down;
        else if (scancode == bindings.rollRight)
            controlKeys.rollRight = down;
    }

    return false;
}

void CameraControls::update(uint64_t deltaMs) {
    auto deltaMsF = static_cast<float>(deltaMs);

    glm::vec3 direction(0.f);

    direction.x = (controlKeys.right ? 1.f : 0.f) + (controlKeys.left ? -1.f : 0.f);
    direction.y = (controlKeys.up ? 1.f : 0.f) + (controlKeys.down ? -1.f : 0.f);
    direction.z = (controlKeys.forward ? 1.f : 0.f) + (controlKeys.back ? -1.f : 0.f);

    camera.moveRelative(direction * deltaMsF * DEFAULT_CAMERA_SPEED);

    float roll = (controlKeys.rollRight ? 1.f : 0.f) + (controlKeys.rollLeft ? -1.f : 0.f);

    if (roll != 0) {
        float rollDelta = roll * deltaMsF * DEFAULT_CAMERA_SENSITIVITY * DEFAULT_CAMERA_SENSITIVITY_KEYBOARD_COEFFICIENT;
        camera.rotate(glm::vec3(0.f, 0.f, rollDelta));
    }
}
