//
// Created by egorv on 4/9/2023.
//

#ifndef SHITCRAFT_CAMERA_H
#define SHITCRAFT_CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "spdlog/spdlog.h"

class Camera {
public:
    explicit Camera(glm::vec3 position = glm::vec3(0.f), glm::vec3 front = glm::vec3(0.f, 0.f, -1.f),
                           glm::vec3 up = glm::vec3(0.f, 1.f, 0.f));

    void moveAbsolute(glm::vec3 delta);

    void moveRelative(glm::vec3 delta);

    void rotate(glm::vec3 angles);

    [[nodiscard]] inline glm::mat4 getViewMatrix() const {
        return viewMatrix;
    }

    [[nodiscard]] inline glm::vec3 getFront() const {
        return camFront;
    }

    [[nodiscard]] inline glm::vec3 getRight() const {
        return camRight;
    }

    [[nodiscard]] inline glm::vec3 getPosition() const {
        return position;
    }

private:
    void update();

    glm::vec3 position, front, up, camFront, camUp, camRight;
    glm::quat qRot = glm::quat(1.f, 0.f, 0.f, 0.f);
    glm::mat4 viewMatrix = glm::mat4(1.f);
};

#endif //SHITCRAFT_CAMERA_H
