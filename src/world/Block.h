//
// Created by egorv on 4/27/2023.
//

#ifndef SHITCRAFT_BLOCK_H
#define SHITCRAFT_BLOCK_H

#include <array>
#include <cstdint>

#include "types.h"

constexpr int AXIS_X = 0;
constexpr int AXIS_Y = 1;
constexpr int AXIS_Z = 2;

constexpr int RIGHT_FACE = 0;
constexpr int LEFT_FACE = 1;
constexpr int TOP_FACE = 2;
constexpr int BOTTOM_FACE = 3;
constexpr int FRONT_FACE = 4;
constexpr int BACK_FACE = 5;

constexpr int face(int axis, bool forwardOrientated) {
    return axis * 2 + !forwardOrientated;
}

/** Checks if face normal is pointing towards positive values **/
constexpr bool isForwardOriented(int face) {
    return face % 2 == 0;
}

struct Block {
public:
    Block();

    [[nodiscard]] constexpr uint getFaceTextureIndex(int face) const {
        return faceTextureIndices[face];
    }

private:
    std::array<uint, 6> faceTextureIndices{};
};


#endif //SHITCRAFT_BLOCK_H
