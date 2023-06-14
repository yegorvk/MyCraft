//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_CHUNKMESHDATABUILDER_H
#define SHITCRAFT_CHUNKMESHDATABUILDER_H

#include <array>
#include <vector>

#include "renderer/Vertex.h"
#include "block/BlockRegistry.h"

#include "Constants.h"
#include "Utils.h"
#include "ChunkMeshData.h"

class ChunkMeshDataBuilder {
public:
    ChunkMeshDataBuilder(const BlockId *blocks, const BlockRegistry &blockCache);

    ChunkMeshData build();
private:
    /**
     * @param originBlockOffset block offset in the direction of positive normal axis
     * @param face block face
     * */
    void build2dMesh(int originBlockOffset, int face);

    [[nodiscard]] inline BlockFace getFace(glm::ivec3 blockCoords, int face) {
        const auto block = getChunkBlock(blocks, blockCoords);
        return ((block == 0) ? BlockFace() : blockCache.getBlock(block).getFace(face));
    }

    [[nodiscard]] inline bool hasTranslucentNeighbour(glm::ivec3 coords, int face) {
        return getAdjacentChunkBlock(blocks, coords, face) == 0;
    }

    const BlockId *blocks;
    const BlockRegistry &blockCache;

    ChunkMeshData meshData;
};


#endif //SHITCRAFT_CHUNKMESHDATABUILDER_H
