//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_CHUNKDATA_H
#define SHITCRAFT_CHUNKDATA_H

#include "registry/BlockRegistry.h"

#include "ChunkMeshData.h"
#include "ChunkBlocks.h"
#include "ChunkMeshDataBuilder.h"

class ChunkData {
public:
    ChunkData() = default;

    [[nodiscard]] inline BlockId getBlock(glm::ivec3 relPosition) const {
        return blocks.getLocalUnchecked(relPosition);
    }

    inline bool setBlock(glm::ivec3 relPosition, BlockId block) {
        if (getBlock(relPosition) == block)
            return false;

        blocks.setLocalUnchecked(relPosition, block);
        return true;
    }

    inline void updateNeighborData(int thisChunkFace, const ChunkData *neighbor) {
        blocks.updateNeighborData(thisChunkFace, neighbor ? &neighbor->blocks : nullptr);
    }

    inline void updateMesh(const BlockRegistry &blockRegistry) {
        meshData = ChunkMeshDataBuilder(blocks, blockRegistry).build();
    }

    [[nodiscard]] inline const ChunkMeshData &getMeshData() const {
        return meshData;
    }

    [[nodiscard]] constexpr bool isEmpty() const {
        return blocks.isEmpty();
    }

private:
    ChunkBlocks blocks;
    ChunkMeshData meshData;
};


#endif //SHITCRAFT_CHUNKDATA_H
