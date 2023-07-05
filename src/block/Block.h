//
// Created by egorv on 4/27/2023.
//

#ifndef SHITCRAFT_BLOCK_H
#define SHITCRAFT_BLOCK_H

#include <array>
#include <cstdint>
#include <glm/glm.hpp>

#include "registry/TextureLookup.h"
#include "types.h"

constexpr int face(int axis, bool forwardOrientated) {
    return axis * 2 + !forwardOrientated;
}

/** Checks if face normal is pointing towards positive values **/
constexpr bool isForwardOriented(int face) {
    return face % 2 == 0;
}

struct BlockFace {
    BlockFace() = default;

    explicit BlockFace(uint16_t textureId);

    [[nodiscard]] inline bool operator==(const BlockFace &other) const {
        return textureId == other.textureId;
    }

    uint16_t textureId = 0;
};

struct Block {
public:
    Block() = default;

    [[nodiscard]] constexpr const BlockFace &getFace(int face) const {
        return faces[face];
    }

    inline void setFace(int face, BlockFace value) {
        faces[face] = value;
    }

private:
    std::array<BlockFace, 6> faces{};
};


#endif //SHITCRAFT_BLOCK_H
