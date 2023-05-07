//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_TRANSFORM_H
#define SHITCRAFT_TRANSFORM_H

#include "ViewFrustrum.h"

struct Transform {
    inline Transform(glm::mat4 transform, ViewFrustrum frustrum)
    : mvp(transform), frustrum(frustrum) {}

    glm::mat4 mvp;
    ViewFrustrum frustrum;
};

#endif //SHITCRAFT_TRANSFORM_H
