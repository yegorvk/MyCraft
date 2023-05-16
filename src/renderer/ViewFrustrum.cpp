//
// Created by egorv on 5/5/2023.
//

#include "spdlog/spdlog.h"

#include "ViewFrustrum.h"

ViewFrustrum::ViewFrustrum(Perspective perspective, glm::dvec3 cameraPosition, glm::dvec3 cameraFront,
                           glm::dvec3 cameraRight) {
    const auto cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    const auto farOffset = cameraFront * perspective.far;

    auto halfFarHeight = perspective.far * glm::tan(perspective.vFovRad / 2.f);
    auto halfFarWidth = halfFarHeight * perspective.aspectRatio;

    near = {cameraPosition + cameraFront * perspective.near, cameraFront};
    far = {cameraPosition + farOffset, -cameraFront};

    right = {cameraPosition, glm::cross(cameraUp, farOffset + halfFarWidth * cameraRight)};
    left = {cameraPosition, glm::cross(farOffset - halfFarWidth * cameraRight, cameraUp)};

    top = {cameraPosition, glm::cross(farOffset + halfFarHeight * cameraUp, cameraRight)};
    bottom = {cameraPosition, glm::cross(farOffset - halfFarHeight * cameraUp, -cameraRight)};
}

BoundingBox::BoundingBox(glm::dvec3 min, glm::dvec3 extents) {
    vertices = {
            min,
            min + glm::dvec3(extents.x, 0, 0),
            min + glm::dvec3(0, extents.y, 0),
            min + glm::dvec3(0, 0, extents.z),
            min + glm::dvec3(0, extents.y, extents.z),
            min + glm::dvec3(extents.x, 0, extents.z),
            min + glm::dvec3(extents.x, extents.y, 0),
            min + glm::dvec3(extents.x, extents.y, extents.z),
    };
}

bool BoundingBox::isOnFrustrum(const ViewFrustrum &frustrum) const {
    return isForwardToPlane(frustrum.near) && isForwardToPlane(frustrum.far) &&
           isForwardToPlane(frustrum.right) && isForwardToPlane(frustrum.left) &&
           isForwardToPlane(frustrum.top) && isForwardToPlane(frustrum.bottom);
}

bool BoundingBox::isForwardToPlane(const FrustrumPlane &plane) const {
    constexpr double EPS = 0.05;

    for (int i = 0; i < 8; ++i) {
        if (plane.signedDistanceFrom(vertices[i]) >= -EPS)
            return true;
    }

    return false;
}