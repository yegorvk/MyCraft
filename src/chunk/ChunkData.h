//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_CHUNKDATA_H
#define SHITCRAFT_CHUNKDATA_H

#include "block/BlockCache.h"

#include "ChunkMeshData.h"
#include "Utils.h"

class ChunkData {
public:
    ChunkData();

    inline void setBlock(glm::ivec3 position, BlockId id) {
        blocks[getBlockIndexInChunk(position)] = id;
    }

    void updateMesh(const BlockCache &blockCache);

    [[nodiscard]] inline const ChunkMeshData &getMeshData() const {
        return meshData;
    }

private:
    std::vector<BlockId> blocks;
    ChunkMeshData meshData;
};


#endif //SHITCRAFT_CHUNKDATA_H
