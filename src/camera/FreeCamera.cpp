//
// Created by egorv on 5/5/2023.
//

#include "FreeCamera.h"

FreeCamera::FreeCamera(glm::vec3 position, glm::vec3 front, glm::vec3 up)
        : position(position), front(glm::normalize(front)), up(glm::normalize(up)), camFront(front), camUp(up),
          camRight(glm::normalize(glm::cross(front, up))) {};

void FreeCamera::moveAbsolute(glm::vec3 delta) {
    position += delta;
    update();
}

void FreeCamera::moveRelative(glm::vec3 delta) {
    position += delta.x * camRight + delta.y * camUp + delta.z * camFront;
    //update();
}

void FreeCamera::rotate(glm::vec3 angles) {
    qRot *= glm::quat(angles);
    update();
}

void FreeCamera::update() {
    camFront = glm::rotate(qRot, front);
    camUp = glm::rotate(qRot, up);
    camRight = glm::normalize(glm::cross(camFront, camUp));

    viewMatrix = glm::lookAt(glm::vec3(0), camFront, camUp);
}

