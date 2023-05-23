//
// Created by egorv on 4/27/2023.
//

#ifndef SHITCRAFT_BLOCK_H
#define SHITCRAFT_BLOCK_H

#include <array>
#include <cstdint>

#include "types.h"

constexpr int face(int axis, bool forwardOrientated) {
    return axis * 2 + !forwardOrientated;
}

/** Checks if face normal is pointing towards positive values **/
constexpr bool isForwardOriented(int face) {
    return face % 2 == 0;
}

struct Block {
public:
    Block(uint rightTex, uint leftTex, uint topTex, uint bottomTex, uint frontTex, uint backTex);

    [[nodiscard]] constexpr uint getFaceTextureIndex(int face) const {
        return faceTextureIndices[face];
    }

private:
    std::array<uint, 6> faceTextureIndices{};
};


#endif //SHITCRAFT_BLOCK_H
