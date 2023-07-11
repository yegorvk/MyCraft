//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_MATHUTILS_H
#define SHITCRAFT_MATHUTILS_H

#include "spdlog/spdlog.h"
#include "glm/glm.hpp"

constexpr glm::ivec3 positiveMod(glm::ivec3 v, glm::ivec3 m)  {
    return (v % m + m) % m;
}

template<typename T>
constexpr bool isPowerOfTwo(T n) {
    return (n & (n - 1)) == 0;
}

template<int N, typename T, glm::qualifier Q>
constexpr T length2(glm::vec<N, T, Q> v) {
    T len2{};

    for (int i = 0; i < N; ++i)
        len2 += v[i] * v[i];

    return len2;
}

inline glm::dvec2 rayIntersectAABB2Points(glm::dvec3 rayOrigin, glm::dvec3 rayDir, glm::dvec3 boxMin, glm::dvec3 boxMax) {
    auto tMin = (boxMin - rayOrigin) / rayDir;
    auto tMax = (boxMax - rayOrigin) / rayDir;
    auto t1 = min(tMin, tMax);
    auto t2 = max(tMin, tMax);
    auto tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
    auto tFar = glm::min(glm::min(t2.x, t2.y), t2.z);
    return glm::vec2(tNear, tFar);
}

inline glm::dvec3 rayIntersectAABB(glm::dvec3 rayOrigin, glm::dvec3 rayDir, glm::dvec3 boxMin, glm::dvec3 boxSize = glm::dvec3(1.0)) {
    double t = rayIntersectAABB2Points(rayOrigin, rayDir, boxMin, boxMin + boxSize).x;
    return rayOrigin + rayDir * t;
}

inline double projectionLength(glm::dvec3 a, glm::dvec3 b) {
    return glm::dot(a, glm::normalize(b));
}

inline double distanceToPlane(glm::dvec3 point, glm::dvec3 pointOnPlain, glm::dvec3 normal) {
    return projectionLength(point - pointOnPlain, normal);
}

int getFaceContainingPointAAB(glm::dvec3 point, glm::dvec3 boxMin, glm::dvec3 boxSize);

#endif //SHITCRAFT_MATHUTILS_H
