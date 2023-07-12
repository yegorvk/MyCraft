//
// Created by egorv on 7/12/2023.
//

#ifndef SHITCRAFT_PERSPECTIVE_H
#define SHITCRAFT_PERSPECTIVE_H

#include "glm/ext.hpp"

struct Perspective {
    [[nodiscard]] inline glm::mat4 computeProjectionMatrix() const {
        return glm::perspective(vFovRad, aspectRatio, near, far);
    }

    double aspectRatio, vFovRad, near, far;
};

#endif //SHITCRAFT_PERSPECTIVE_H
