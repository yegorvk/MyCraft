//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_CHUNKMESHDATABUILDER_H
#define SHITCRAFT_CHUNKMESHDATABUILDER_H

#include <array>
#include <vector>

#include "renderer/Vertex.h"
#include "block/BlockCache.h"

#include "Constants.h"
#include "Utils.h"
#include "ChunkMeshData.h"

class ChunkMeshDataBuilder {
public:
    ChunkMeshDataBuilder(const BlockId *blocks, const BlockCache &blockCache);

    ChunkMeshData build();
private:
    /**
     * @param originBlockOffset block offset in the direction of positive normal axis
     * @param face block face
     * */
    void build2dMesh(int originBlockOffset, int face);

    [[nodiscard]] inline ALWAYS_INLINE uint getTexId(glm::ivec3 blockCoords, int face) {
        auto block = getChunkBlock(blocks, blockCoords);
        return ((block == 0) ? 0 : blockCache.getBlock(block).getFaceTextureIndex(face));
    }

    [[nodiscard]] inline ALWAYS_INLINE bool hasTranslucentNeighbour(glm::ivec3 coords, int face) {
        return getAdjacentChunkBlock(blocks, coords, face) == 0;
    }

    const BlockId *blocks;
    const BlockCache &blockCache;

    ChunkMeshData meshData;
};


#endif //SHITCRAFT_CHUNKMESHDATABUILDER_H
