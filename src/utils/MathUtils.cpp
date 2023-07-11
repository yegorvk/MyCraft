//
// Created by egorv on 7/11/2023.
//

#include "chunk/Constants.h"
#include "MathUtils.h"

int getFaceContainingPointAAB(glm::dvec3 point, glm::dvec3 boxMin, glm::dvec3 boxSize) {
    const auto boxMax = boxMin + boxSize;
    double minDst = 1e6;
    int minDstFace = -1;

    for (int i = 0; i < 6; ++i) {
        auto pointOnFace = (i & 1) ? boxMin : boxMax;
        auto dst = distanceToPlane(point, pointOnFace, -Face::getNormalDirection(i));

        if (dst < minDst) {
            minDst = dst;
            minDstFace = i;
        }
    }

    return minDstFace;
}