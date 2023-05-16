//
// Created by egorv on 5/7/2023.
//

#ifndef SHITCRAFT_AAB_H
#define SHITCRAFT_AAB_H

#include "glm/glm.hpp"

struct AAB {
    constexpr explicit AAB(glm::ivec3 min = {}, glm::ivec3 max = {})
            : min(glm::min(min, max)), max(glm::max(min, max)) {}

    [[nodiscard]] constexpr glm::ivec3 getMin() const {
        return min;
    }

    [[nodiscard]] constexpr glm::ivec3 getMax() const {
        return max;
    }

    [[nodiscard]] constexpr int getVolume() const {
        const auto extents = max - min;
        return extents.x * extents.y * extents.z;
    }

    [[nodiscard]] constexpr AAB intersectWith(AAB other) const {
        const auto iMin = glm::max(min, other.min);
        const auto iMax = glm::min(max, other.max);
        return AAB(iMin, iMax);
    }

private:
    glm::ivec3 min, max;
};

#endif //SHITCRAFT_AAB_H
