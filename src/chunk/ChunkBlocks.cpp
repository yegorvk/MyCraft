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
    const auto normal = Face::getNormalAxisDirection(face);
    const auto adjPosition = position + normal;

    if (isWithinChunk(adjPosition))
        return getLocalUnchecked(adjPosition);
    else {
        return adjFaces[face][getProjIndex(position, face)];
    }
}

void ChunkBlocks::updateNeighborData(int thisChunkFace, const ChunkBlocks &neighbor) {
    const auto origin = Face::getBorderFaceOrigin(thisChunkFace);
    const int normalAxis = Face::getNormalAxis(thisChunkFace);

    glm::ivec3 size(CHUNK_SIDE_BLOCK_COUNT);
    size[Face::getNormalAxis(thisChunkFace)] = 1;

    for (int x = origin.x; x < size.x; ++x) {
        for (int y = origin.y; y < size.y; ++y) {
            for (int z = origin.z; z < size.z; ++z) {
                const glm::ivec3 pos(x, y, z);
                const auto mirroredPos = mirrorPosition(pos, normalAxis);

                adjFaces[thisChunkFace][getProjIndex(pos, thisChunkFace)] = neighbor.getLocalUnchecked(mirroredPos);
            }
        }
    }
}

