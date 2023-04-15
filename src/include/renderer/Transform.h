//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_TRANSFORM_H
#define SHITCRAFT_TRANSFORM_H

#include "glm/glm.hpp"

struct Transform {
    inline explicit Transform(glm::mat4 transform) : transform(transform) {}

    glm::mat4 transform;
};

#endif //SHITCRAFT_TRANSFORM_H
