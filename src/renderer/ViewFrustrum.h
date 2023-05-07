//
// Created by egorv on 5/3/2023.
//

#ifndef SHITCRAFT_VIEWFRUSTRUM_H
#define SHITCRAFT_VIEWFRUSTRUM_H

#include <array>

#include "glm/glm.hpp"

struct FrustrumPlane {
    FrustrumPlane() = default;

    inline FrustrumPlane(glm::vec3 point, glm::vec3 normal)
    : point(point), normal(glm::normalize(normal)) {}

    [[nodiscard]] inline float signedDistanceFrom(glm::vec3 p) const {
        return glm::dot(normal, p - point);
    }

    glm::vec3 point{}, normal{};
};

struct Perspective {
    float aspectRatio, vFovRad, near, far;
};

struct ViewFrustrum {
    ViewFrustrum(Perspective perspective, glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraRight);

    FrustrumPlane near, far, right, left, top, bottom;
};

class BoundingBox {
public:
    BoundingBox(glm::vec3 min, glm::vec3 extents);

    [[nodiscard]] bool isOnFrustrum(const ViewFrustrum &frustrum) const;
private:
    [[nodiscard]] bool isForwardToPlane(const FrustrumPlane &plane) const;

    std::array<glm::vec3, 8> vertices{};
};

#endif //SHITCRAFT_VIEWFRUSTRUM_H
