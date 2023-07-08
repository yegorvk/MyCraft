//
// Created by egorv on 4/14/2023.
//

#ifndef SHITCRAFT_CHUNKVERTEX_H
#define SHITCRAFT_CHUNKVERTEX_H

#include "glm/glm.hpp"
#include "types.h"

constexpr uint32_t MASK_ID = 0xFFF;
constexpr uint32_t MASK_X = 0x1F;
constexpr uint32_t MASK_Y = 0x1F;
constexpr uint32_t MASK_Z = 0x1F;
constexpr uint32_t MASK_AO = 0x3;

constexpr uint32_t SHIFT_ID = 17;
constexpr uint32_t SHIFT_X = 12;
constexpr uint32_t SHIFT_Y = 7;
constexpr uint32_t SHIFT_Z = 2;
constexpr uint32_t SHIFT_AO = 0;

constexpr uint32_t MASK_U = 0x1F;
constexpr uint32_t MASK_V = 0x1F;

constexpr uint32_t SHIFT_U = 5;
constexpr uint32_t SHIFT_V = 0;

struct PackedChunkVertex {
    constexpr PackedChunkVertex() = default;

    constexpr PackedChunkVertex(glm::uvec3 position, glm::uvec2 texCoords, int face, int textureId, int ao) {
        v.x = (textureId << SHIFT_ID) + (position.x << SHIFT_X) + (position.y << SHIFT_Y) +
              (position.z << SHIFT_Z) + (ao << SHIFT_AO);

        v.y = (texCoords.s << SHIFT_U) + (texCoords.t << SHIFT_V);
    }

    glm::uvec2 v{};
};

#endif //SHITCRAFT_CHUNKVERTEX_H
