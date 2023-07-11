//
// Created by egorv on 7/5/2023.
//

#ifndef SHITCRAFT_COMPOSITERENDERER_H
#define SHITCRAFT_COMPOSITERENDERER_H

#include "glm/glm.hpp"

#include "gl/GlResource.h"
#include "ViewFrustrum.h"
#include "ChunkRenderer.h"
#include "HUDRenderer.h"

struct RenderState {
    ViewFrustrum frustrum;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::dvec3 playerPosition;
};

class CompositeRenderer {
public:
    CompositeRenderer();

    void resize(glm::ivec2 newViewportSize);

    void draw(const RenderState &state);

    inline void resetActiveChunks(glm::ivec3 newActiveRegionMin, glm::ivec3 newActiveRegionSize) {
        chunkRenderer.reset(newActiveRegionMin, newActiveRegionSize);
    }

    inline void setActiveChunksMin(glm::ivec3 min) {
        chunkRenderer.setActiveRegionMin(min);
    }

    inline void updateChunk(glm::ivec3 chunkPos, const ChunkMeshData *meshData) {
        chunkRenderer.update(chunkPos, meshData);
    }

private:
    ChunkRenderer chunkRenderer;
    HUDRenderer hudRenderer;

    glm::ivec2 viewportSize{};
};


#endif //SHITCRAFT_COMPOSITERENDERER_H
