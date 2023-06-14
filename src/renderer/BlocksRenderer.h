//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_BLOCKSRENDERER_H
#define SHITCRAFT_BLOCKSRENDERER_H

#include "TextureManager.h"
#include "ChunkMesh.h"
#include "world/World.h"
#include "chunk/ChunkMeshData.h"
#include "ViewFrustrum.h"
#include "MathUtils.h"
#include "ArrayUtils.h"

class BlocksRenderer {
public:
    BlocksRenderer();

    void draw(glm::dvec3 cameraPosition, const glm::mat4 &viewProjection, const ViewFrustrum &frustrum) const;

    void reset(glm::ivec3 newActiveRegionMin, glm::ivec3 newActiveRegionSize);

    inline void setActiveRegionMin(glm::ivec3 min) {
        activeRegionMin = min;
    }

    void update(glm::ivec3 chunkPos, const ChunkMeshData &meshData);

private:
    [[nodiscard]] inline ChunkMesh &getMesh(glm::ivec3 position) {
        return chunkMeshes[flatten(positiveMod(position, activeRegionSize), activeRegionSize)];
    }

    [[nodiscard]] inline const ChunkMesh &getMesh(glm::ivec3 position) const {
        return chunkMeshes[flatten(positiveMod(position, activeRegionSize), activeRegionSize)];
    }

    const Shader *shader = nullptr;

    glm::ivec3 activeRegionMin{}, activeRegionSize{};

    std::vector<ChunkMesh> chunkMeshes;

    Texture texture;
};


#endif //SHITCRAFT_BLOCKSRENDERER_H
