//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_MATHUTILS_H
#define SHITCRAFT_MATHUTILS_H

#include "glm/glm.hpp"

constexpr glm::ivec3 positiveMod(glm::ivec3 v, glm::ivec3 m)  {
    return (v % m + m) % m;
}

constexpr int wrapIndex(glm::ivec3 i, glm::ivec3 size) {
    const auto wrapped = positiveMod(i, size);
    return wrapped.x * size.y * size.z + wrapped.y * size.z + wrapped.z;
}

#endif //SHITCRAFT_MATHUTILS_H
