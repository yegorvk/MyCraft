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
    inline explicit Camera(
            glm::vec3 position = glm::vec3(0.f),
            glm::vec3 front = glm::vec3(0.f, 0.f, -1.f),
            glm::vec3 up = glm::vec3(0.f, 1.f, 0.f))
            : position(position), front(glm::normalize(front)), up(glm::normalize(up)),
              camFront(front), camUp(up), camRight(glm::normalize(glm::cross(front, up))) {}

    inline void moveAbsolute(glm::vec3 delta) {
        position += delta;
        update();
    }

    inline void moveRelative(glm::vec3 delta) {
        position += camRight * delta.x + camUp * delta.y + camFront * delta.z;
        update();
    }

    inline void rotate(glm::vec3 angles) {
        qRot *= glm::conjugate(glm::quat(angles));
        update();
    }

    [[nodiscard]] inline glm::mat4 getViewMatrix() const {
        return viewMatrix;
    }
private:
    inline void update() {
        camFront = glm::rotate(qRot, front);
        camUp = glm::rotate(qRot, up);
        camRight = glm::normalize(glm::cross(camFront, camUp));

        viewMatrix = glm::lookAt(position, position + camFront, camUp);
    }

    glm::vec3 position, front, up, camFront, camUp, camRight;
    glm::quat qRot = glm::quat(1.f, 0.f, 0.f, 0.f);
    glm::mat4 viewMatrix = glm::mat4(1.f);
};

#endif //SHITCRAFT_CAMERA_H
