//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_IOUTILS_H
#define SHITCRAFT_IOUTILS_H

#include <glm/glm.hpp>

#include "Constants.h"
#include "arch.h"

constexpr int getBlockIndexInChunk(glm::ivec3 position) {
    return position.x * CHUNK_SIDE_BLOCK_COUNT * CHUNK_SIDE_BLOCK_COUNT +
           position.y * CHUNK_SIDE_BLOCK_COUNT +
           position.z;
}

constexpr bool isWithinChunkBounds(int position) {
    return position >= 0 && position < CHUNK_SIDE_BLOCK_COUNT;
}

constexpr bool isWithinChunkBounds(glm::ivec3 position) {
    return isWithinChunkBounds(position.x) &&
           isWithinChunkBounds(position.y) &&
           isWithinChunkBounds(position.z);
}

constexpr glm::ivec3 getAdjacentBlockPosition(glm::ivec3 position, int face) {
    return position + Face::getNormalAxisDirection(face);
}

template<typename Blocks>
inline ALWAYS_INLINE BlockId getChunkBlock(const Blocks &blocks, glm::ivec3 position) {
    return blocks[getBlockIndexInChunk(position)];
}

template<typename Blocks, BlockId defaultValue = 0>
inline ALWAYS_INLINE BlockId getChunkBlockChecked(const Blocks &blocks, glm::ivec3 position) {
    return isWithinChunkBounds(position) ? getChunkBlock(blocks, position) : defaultValue;
}

template<typename Blocks>
constexpr BlockId getAdjacentChunkBlock(const Blocks &blocks, glm::ivec3 position, int face) {
    return getChunkBlockChecked(blocks, getAdjacentBlockPosition(position, face));
}

#endif //SHITCRAFT_IOUTILS_H
