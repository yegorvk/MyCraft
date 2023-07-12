//
// Created by egorv on 5/3/2023.
//

#ifndef SHITCRAFT_VIEWFRUSTRUM_H
#define SHITCRAFT_VIEWFRUSTRUM_H

#include <array>

#include "glm/glm.hpp"

#include "Perspective.h"

struct FrustrumPlane {
    FrustrumPlane() = default;

    inline FrustrumPlane(glm::dvec3 point, glm::dvec3 normal)
    : point(point), normal(glm::normalize(normal)) {}

    [[nodiscard]] inline double signedDistanceFrom(glm::dvec3 p) const {
        return glm::dot(normal, p - point);
    }

    glm::dvec3 point{}, normal{};
};

struct ViewFrustrum {
    ViewFrustrum(Perspective perspective, glm::dvec3 cameraPosition, glm::dvec3 cameraFront, glm::dvec3 cameraRight);

    FrustrumPlane near, far, right, left, top, bottom;
};

class BoundingBox {
public:
    BoundingBox(glm::dvec3 min, glm::dvec3 extents);

    [[nodiscard]] bool isOnFrustrum(const ViewFrustrum &frustrum) const;
private:
    [[nodiscard]] bool isForwardToPlane(const FrustrumPlane &plane) const;

    std::array<glm::dvec3, 8> vertices{};
};

#endif //SHITCRAFT_VIEWFRUSTRUM_H
