//
// Created by egorv on 4/27/2023.
//

#ifndef SHITCRAFT_BLOCK_H
#define SHITCRAFT_BLOCK_H

#include <array>
#include <cstdint>

#include "types.h"

enum class BlockFace : uint8_t {
    Front = 0, // z+
    Back = 1, // z-
    Right = 2, // x+
    Left = 3, // x-
    Top = 4, // y+
    Bottom = 5 // y-
};

struct Block {
public:
    Block();

    constexpr uint getFaceTextureIndex(BlockFace face) const {
        return faceTextureIndices[static_cast<std::size_t>(face)];
    }

private:
    std::array<uint, 6> faceTextureIndices{};
};


#endif //SHITCRAFT_BLOCK_H
