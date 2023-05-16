//
// Created by egorv on 4/29/2023.
//

#ifndef SHITCRAFT_CHUNKMESHBUILDER_H
#define SHITCRAFT_CHUNKMESHBUILDER_H

#include <vector>
#include <array>

#include "Vertex.h"
#include "arch.h"
#include "world/Chunk.h"
#include "world/BlockCache.h"

class ChunkMeshBuilder {
public:
    ChunkMeshBuilder(const Chunk &chunk, const BlockCache &blockCache, glm::vec3 offset, float blockSideLen);

    [[nodiscard]] inline const std::vector<Vertex> &getVertices(int face) const {
        return vertices[face];
    }

    [[nodiscard]] int getTotalVertexCount() const;
private:
    /**
     * @param originBlockOffset block offset in the direction of positive normal axis
     * @param face block face
     * @param normalAxis face normal axis
     * */
    void build2dMesh(int originBlockOffset, int face, int normalAxis);

    [[nodiscard]] inline ALWAYS_INLINE std::vector<Vertex> &getVerticesMut(int face) {
        return vertices[face];
    }

    [[nodiscard]] inline ALWAYS_INLINE uint getTexId(glm::ivec3 blockCoords, int face) {
        auto block = chunk.getBlock(blockCoords);
        return ((block == 0) ? 0 : blockCache.getBlock(block).getFaceTextureIndex(face));
    }

    [[nodiscard]] inline ALWAYS_INLINE bool hasTranslucentNeighbour(glm::ivec3 coords, int face) {
        return chunk.getBlockChecked(coords + ADJ_BLOCK[face]) == 0;
    }

    static constexpr glm::ivec3 ADJ_BLOCK[6] = {
            {1,  0,  0},
            {-1, 0,  0},
            {0,  1,  0},
            {0,  -1, 0},
            {0,  0,  1},
            {0,  0,  -1},
    };

    const Chunk &chunk;
    const BlockCache &blockCache;

    glm::vec3 offset;
    float blockSideLen;

    std::array<std::vector<Vertex>, 6> vertices;
};


#endif //SHITCRAFT_CHUNKMESHBUILDER_H
