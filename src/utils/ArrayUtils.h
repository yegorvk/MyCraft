//
// Created by egorv on 5/19/2023.
//

#ifndef SHITCRAFT_ARRAYUTILS_H
#define SHITCRAFT_ARRAYUTILS_H

#include "glm/glm.hpp"

constexpr int flatten(glm::ivec3 index, glm::ivec3 size) {
    return index.x * size.y * size.z + index.y * size.z + index.z;
}

constexpr int flatten(glm::ivec3 index, int dimSize) {
    return flatten(index, glm::ivec3(dimSize));
}

#endif //SHITCRAFT_ARRAYUTILS_H
