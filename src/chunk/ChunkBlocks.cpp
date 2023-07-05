//
// Created by egorv on 6/18/2023.
//

#include "ChunkBlocks.h"

constexpr bool isWithinChunk(int position) {
    return position >= 0 && position < CHUNK_SIDE_BLOCK_COUNT;
}

constexpr bool isWithinChunk(glm::ivec3 position) {
    return isWithinChunk(position.x) && isWithinChunk(position.y) && isWithinChunk(position.z);
}

constexpr glm::ivec2 getProjCoords(glm::ivec3 position, int face) {
    switch (Axis::toEnum(Face::getNormalAxis(face))) {
        case AxisEnum::X:
            return {position.y, position.z};
        case AxisEnum::Y:
            return {position.x, position.z};
        case AxisEnum::Z:
            return {position.x, position.y};
    }
}

constexpr int getProjIndex(glm::ivec3 position, int face) {
    const auto projPos = getProjCoords(position, face);
    return projPos.x * CHUNK_SIDE_BLOCK_COUNT + projPos.y;
}

constexpr glm::ivec3 mirrorPosition(glm::ivec3 position, int axis) {
    position[axis] = CHUNK_SIDE_BLOCK_COUNT - 1 - position[axis];
    return position;
}

ChunkBlocks::ChunkBlocks() {
    blocks.resize(CHUNK_SIDE_BLOCK_COUNT * CHUNK_SIDE_BLOCK_COUNT * CHUNK_SIDE_BLOCK_COUNT, 0);

    for (int i = 0; i < 6; ++i)
        adjFaces[i].resize(CHUNK_SIDE_BLOCK_COUNT * CHUNK_SIDE_BLOCK_COUNT, 0);
}

BlockId ChunkBlocks::getAdjacentTo(glm::ivec3 position, int face) const {
    const auto normal = Face::getNormalDirection(face);
    const auto adjPosition = position + normal;

    if (isWithinChunk(adjPosition))
        return getLocalUnchecked(adjPosition);
    else {
        return adjFaces[face][getProjIndex(position, face)];
    }
}

BlockId ChunkBlocks::getLocalChecked(glm::ivec3 position) const {
    if (isWithinChunk(position))
        return getLocalUnchecked(position);
    else
        return 0;
}

void ChunkBlocks::updateNeighborData(int thisChunkFace, const ChunkBlocks *neighbor) {
    const int normalAxis = Face::getNormalAxis(thisChunkFace);
    const bool positiveOrientated = Face::isPositiveOrientated(thisChunkFace);

    glm::ivec3 min(0);

    if (positiveOrientated)
        min[normalAxis] = CHUNK_SIDE_BLOCK_COUNT - 1;

    glm::ivec3 max(CHUNK_SIDE_BLOCK_COUNT - 1);

    if (!positiveOrientated)
        max[normalAxis] = 0;

    for (int x = min.x; x <= max.x; ++x) {
        for (int y = min.y; y <= max.y; ++y) {
            for (int z = min.z; z <= max.z; ++z) {
                const glm::ivec3 pos(x, y, z);
                const auto mirroredPos = mirrorPosition(pos, normalAxis);

                adjFaces[thisChunkFace][getProjIndex(pos, thisChunkFace)] =
                        neighbor ? neighbor->getLocalUnchecked(mirroredPos) : 0;
            }
        }
    }
}

