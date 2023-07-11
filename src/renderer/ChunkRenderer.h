//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_CHUNKRENDERER_H
#define SHITCRAFT_CHUNKRENDERER_H

#include "registry/TextureManager.h"
#include "mesh/ChunkMesh.h"
#include "world/World.h"
#include "gl/Shader.h"
#include "chunk/ChunkMeshData.h"
#include "ViewFrustrum.h"
#include "utils/MathUtils.h"
#include "utils/ArrayUtils.h"

struct ChunkRenderState {
    glm::dvec3 playerPosition;
    glm::mat4 viewProjection;
    ViewFrustrum frustrum;
};

class ChunkRenderer {
public:
    ChunkRenderer();

    void draw(const ChunkRenderState &state) const;

    void reset(glm::ivec3 newActiveRegionMin, glm::ivec3 newActiveRegionSize);

    inline void setActiveRegionMin(glm::ivec3 min) {
        activeRegionMin = min;
    }

    void update(glm::ivec3 chunkPos, const ChunkMeshData *meshData);

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

    Texture2dArray texture;
};


#endif //SHITCRAFT_CHUNKRENDERER_H
