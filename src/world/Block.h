//
// Created by egorv on 4/27/2023.
//

#ifndef SHITCRAFT_BLOCK_H
#define SHITCRAFT_BLOCK_H

#include <array>
#include <cstdint>

#include "types.h"

enum class BlockFace : uint8_t {
    Front = 0,
    Back = 1,
    Left = 2,
    Right = 3,
    Top = 4,
    Bottom = 5
};

struct Block {
public:
    Block();

    constexpr uint getFaceTextureIndex(BlockFace face) {
        return faceTextureIndices[static_cast<std::size_t>(face)];
    }

private:
    std::array<uint, 6> faceTextureIndices{};
};


#endif //SHITCRAFT_BLOCK_H
