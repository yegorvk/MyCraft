//
// Created by egorv on 4/27/2023.
//

#ifndef SHITCRAFT_BLOCK_H
#define SHITCRAFT_BLOCK_H

#include <array>
#include <cstdint>

#include "types.h"

enum class Axis : int {
    X = 0,
    Y = 1,
    Z = 2
};

enum class BlockFace : int {
    Right = 0, // x+
    Left = 1, // x-
    Top = 2, // y+
    Bottom = 3, // y-
    Front = 4, // z+
    Back = 5 // z-
};

constexpr BlockFace face(Axis axis, bool visibleWhenLookingInPositiveDirection) {
    return static_cast<BlockFace>(static_cast<int>(axis) * 2 + visibleWhenLookingInPositiveDirection);
}

/** Checks if face is visible when looking in positive direction of normal axis **/
constexpr bool faceOrientationAlongNormalAxis(BlockFace face) {
    return static_cast<int>(face) % 2 == 0;
}

struct Block {
public:
    Block();

    [[nodiscard]] constexpr uint getFaceTextureIndex(BlockFace face) const {
        return faceTextureIndices[static_cast<std::size_t>(face)];
    }

private:
    std::array<uint, 6> faceTextureIndices{};
};


#endif //SHITCRAFT_BLOCK_H
