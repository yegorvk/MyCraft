//
// Created by egorv on 4/14/2023.
//

#ifndef SHITCRAFT_CHUNKVERTEX_H
#define SHITCRAFT_CHUNKVERTEX_H

#include "glm/glm.hpp"
#include "types.h"

constexpr uint32_t VERT0_ID_MASK = 0xFFF00000;
constexpr uint32_t VERT0_OFFSETS_MASK = 0xF0000;
constexpr uint32_t VERT0_X_MASK = 0xF000;
constexpr uint32_t VERT0_Y_MASK = 0xF00;
constexpr uint32_t VERT0_Z_MASK = 0xF0;
constexpr uint32_t VERT0_AO_MASK = 0xF;

constexpr uint32_t VERT0_ID_SHIFT = 20;
constexpr uint32_t VERT0_OFFSETS_SHIFT = 16;
constexpr uint32_t VERT0_X_SHIFT = 12;
constexpr uint32_t VERT0_Y_SHIFT = 8;
constexpr uint32_t VERT0_Z_SHIFT = 4;
constexpr uint32_t VERT0_AO_SHIFT = 0;

constexpr uint32_t VERT1_U_MASK = 0x3E0;
constexpr uint32_t VERT1_V_MASK = 0x1F;

constexpr uint32_t VERT1_U_SHIFT = 5;
constexpr uint32_t VERT1_V_SHIFT = 0;

struct PackedChunkVertex {
    constexpr PackedChunkVertex() = default;

    constexpr PackedChunkVertex(glm::uvec3 blockCoords, glm::bvec3 offs, glm::uvec2 texCoords, int face, int textureId, int ao) {
        assert(blockCoords.x < 16 && blockCoords.y < 16 && blockCoords.z < 16);
        assert(texCoords.x <= 16 && texCoords.y <= 16);
        assert(ao < 4);
        assert(textureId > 0);

        uint offMask = (offs.x << 2) + (offs.y << 1) + offs.z;

        v.x = (textureId << VERT0_ID_SHIFT) + (offMask << VERT0_OFFSETS_SHIFT) + (blockCoords.x << VERT0_X_SHIFT) +
              (blockCoords.y << VERT0_Y_SHIFT) + (blockCoords.z << VERT0_Z_SHIFT) + (ao << VERT0_AO_SHIFT);

        v.y = (texCoords.s << VERT1_U_SHIFT) + (texCoords.t << VERT1_V_SHIFT);
    }

    glm::uvec2 v{};
};

#endif //SHITCRAFT_CHUNKVERTEX_H
