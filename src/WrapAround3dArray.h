//
// Created by egorv on 5/15/2023.
//

#ifndef SHITCRAFT_WRAPAROUND3DARRAY_H
#define SHITCRAFT_WRAPAROUND3DARRAY_H

#include <vector>

#include "glm/glm.hpp"

#include "AAB.h"

template<typename T, typename Container = std::vector<T>>
class WrapAround3dArray {
public:
    constexpr WrapAround3dArray() = default;

    constexpr explicit WrapAround3dArray(glm::ivec3 size)
            : data(size.x * size.y * size.z), size(size) {};

    constexpr void reset(glm::ivec3 newSize) {
        size = newSize;

        data.clear();
        data.resize(size.x * size.y * size.z);
    }

    [[nodiscard]] constexpr const T &get(glm::ivec3 index) const {
        return data[getIndex(index)];
    }

    [[nodiscard]] constexpr T &get(glm::ivec3 index) {
        return data[getIndex(index)];
    }

    template<typename F>
    constexpr void forEach(F &&consumer, AAB region) {
        if (size.x == 0 || size.y == 0 || size.z == 0)
            return;

        const auto min = region.getMin(), max = region.getMax();

        for (int x = min.x; x <= max.x; ++x)
            for (int y = min.y; y <= max.y; ++y)
                for (int z = min.z; z <= max.z; ++z)
                    consumer(glm::ivec3(x, y, z), data[getIndex(x, y, z)]);
    }

    template<typename F>
    constexpr void forEach(F &&consumer, AAB region) const {
        if (size.x == 0 || size.y == 0 || size.z == 0)
            return;

        const auto min = region.getMin(), max = region.getMax();

        for (int x = min.x; x <= max.x; ++x)
            for (int y = min.y; y <= max.y; ++y)
                for (int z = min.z; z <= max.z; ++z)
                    consumer(glm::ivec3(x, y, z), data[getIndex(x, y, z)]);
    }

private:
    [[nodiscard]] constexpr int getIndex(glm::ivec3 rawIndex) const {
        return getIndex(rawIndex.x, rawIndex.y, rawIndex.z);
    }

    [[nodiscard]] constexpr int getIndex(int x, int y, int z) const {
        return (x % size.x) * size.y * size.z + (y % size.y) * size.z + (z % size.x);
    }

    glm::ivec3 size{};
    Container data;
};


#endif //SHITCRAFT_WRAPAROUND3DARRAY_H
