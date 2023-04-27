//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_CHUNK_H
#define SHITCRAFT_CHUNK_H

#include <cstdint>
#include <vector>

#include "arch.h"

class Chunk {
public:
    explicit Chunk(int size);

    [[nodiscard]] ALWAYS_INLINE inline uint16_t getBlock(int x, int y, int z) const {
        return (x * sideLen * sideLen) + (y * sideLen) + z;
    }

    [[nodiscard]] ALWAYS_INLINE inline int getSideLen() const {
        return sideLen;
    }

private:
    int sideLen = 0;
    std::vector<uint16_t> blocks;
};

#endif //SHITCRAFT_CHUNK_H
