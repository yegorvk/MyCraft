//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_MATHUTILS_H
#define SHITCRAFT_MATHUTILS_H

#include "glm/glm.hpp"

constexpr glm::ivec3 positiveMod(glm::ivec3 v, glm::ivec3 m)  {
    return (v % m + m) % m;
}

template<typename T>
constexpr bool isPowerOfTwo(T n) {
    return (n & (n - 1)) == 0;
}

#endif //SHITCRAFT_MATHUTILS_H
