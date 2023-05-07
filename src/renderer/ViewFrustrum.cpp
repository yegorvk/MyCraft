//
// Created by egorv on 5/5/2023.
//

#include "spdlog/spdlog.h"

#include "ViewFrustrum.h"

ViewFrustrum::ViewFrustrum(Perspective perspective, glm::vec3 cameraPosition, glm::vec3 cameraFront,
                           glm::vec3 cameraRight) {
    const auto cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    const auto farOffset = cameraFront * perspective.far;

    float halfFarHeight = perspective.far * glm::tan(perspective.vFovRad / 2.f);
    float halfFarWidth = halfFarHeight * perspective.aspectRatio;

    near = {cameraPosition + cameraFront * perspective.near, cameraFront};
    far = {cameraPosition + farOffset, -cameraFront};

    right = {cameraPosition, glm::cross(cameraUp, farOffset + halfFarWidth * cameraRight)};
    left = {cameraPosition, glm::cross(farOffset - halfFarWidth * cameraRight, cameraUp)};

    top = {cameraPosition, glm::cross(farOffset + halfFarHeight * cameraUp, cameraRight)};
    bottom = {cameraPosition, glm::cross(farOffset - halfFarHeight * cameraUp, -cameraRight)};
}

BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 extents) {
    vertices = {
            min,
            min + glm::vec3(extents.x, 0.f, 0.f),
            min + glm::vec3(0.f, extents.y, 0.f),
            min + glm::vec3(0.f, 0.f, extents.z),
            min + glm::vec3(0.f, extents.y, extents.z),
            min + glm::vec3(extents.x, 0.f, extents.z),
            min + glm::vec3(extents.x, extents.y, 0.f),
            min + glm::vec3(extents.x, extents.y, extents.z),
    };
}

bool BoundingBox::isOnFrustrum(const ViewFrustrum &frustrum) const {
    return isForwardToPlane(frustrum.near) && isForwardToPlane(frustrum.far) &&
           isForwardToPlane(frustrum.right) && isForwardToPlane(frustrum.left) &&
           isForwardToPlane(frustrum.top) && isForwardToPlane(frustrum.bottom);
}

bool BoundingBox::isForwardToPlane(const FrustrumPlane &plane) const {
    constexpr float EPS = 0.05f;

    for (int i = 0; i < 8; ++i) {
        if (plane.signedDistanceFrom(vertices[i]) >= -EPS)
            return true;
    }

    return false;
}