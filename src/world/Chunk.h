//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_CHUNK_H
#define SHITCRAFT_CHUNK_H

#include <cstdint>
#include <vector>

#include "glm/glm.hpp"

#include "arch.h"

constexpr int CHUNK_SIDE_BLOCK_COUNT_LOG2 = 4;

class Chunk {
public:
    explicit Chunk(int sideLenLog2 = CHUNK_SIDE_BLOCK_COUNT_LOG2);

    [[nodiscard]] ALWAYS_INLINE inline uint16_t getBlock(int x, int y, int z) const {
        return blocks[(x << sideBlockCountLog2 << sideBlockCountLog2) + (y << sideBlockCountLog2) + z];
    }

    [[nodiscard]] ALWAYS_INLINE inline uint16_t getBlock(glm::ivec3 coords) const {
        return getBlock(coords.x, coords.y, coords.z);
    }

    [[nodiscard]] ALWAYS_INLINE inline uint16_t getBlockChecked(int x, int y, int z) const {
        if (!validateCoords({x, y, z}))
            return 0;

        return getBlock(x, y, z);
    }

    [[nodiscard]] ALWAYS_INLINE inline uint16_t getBlockChecked(glm::ivec3 coords) const {
        return getBlockChecked(coords.x, coords.y, coords.z);
    }

    [[nodiscard]] ALWAYS_INLINE constexpr bool validateCoords(glm::ivec3 coords) const {
        int x = coords.x, y = coords.y, z = coords.z;
        return !(x < 0 || x >= getSideBlockCount() || y < 0 || y >= getSideBlockCount() || z < 0 ||
               z >= getSideBlockCount());
    }

    [[nodiscard]] ALWAYS_INLINE inline int getSideBlockCountLog2() const {
        return sideBlockCountLog2;
    }

    [[nodiscard]] ALWAYS_INLINE inline int getSideBlockCount() const {
        return 1 << sideBlockCountLog2;
    }

    [[nodiscard]] ALWAYS_INLINE inline int getFaceBlockCount() const {
        return 1 << sideBlockCountLog2 << sideBlockCountLog2;
    }

    [[nodiscard]] ALWAYS_INLINE inline int getBlockCount() const {
        return 1 << sideBlockCountLog2 << sideBlockCountLog2;
    }

private:
    int sideBlockCountLog2 = 0;
    std::vector<uint16_t> blocks;
};

#endif //SHITCRAFT_CHUNK_H
