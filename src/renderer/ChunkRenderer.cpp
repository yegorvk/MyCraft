//
// Created by egorv on 4/26/2023.
//

#include "ViewFrustrum.h"
#include "GameContext.h"
#include "chunk/Constants.h"
#include "ChunkRenderer.h"

ChunkRenderer::ChunkRenderer()
        : shader(&GameContext::global().getAssets().getShader("@shader/solid")),
          texture(GameContext::global().getBlockRegistry().getTextureManager().createArrayTexture(
                  Tex2dDesc(16, 16, 4), TexSamplerOptions::linearMipmapNearestRepeat())) {}

void ChunkRenderer::reset(glm::ivec3 newActiveRegionMin, glm::ivec3 newActiveRegionSize) {
    activeRegionSize = newActiveRegionSize, activeRegionMin = newActiveRegionMin;

    chunkMeshes.clear();
    chunkMeshes.resize(newActiveRegionSize.x * newActiveRegionSize.y * newActiveRegionSize.z);
}

void ChunkRenderer::update(glm::ivec3 chunkPos, const ChunkMeshData *meshData) {
    auto &mesh = getMesh(chunkPos);
    mesh.update(meshData);
}

void
ChunkRenderer::draw(const ChunkRenderState &state) const {
    texture.bind();
    shader->bind();

    shader->setFloat("blockScale", BLOCK_SIDE_SCALE_F);

    for (int i = activeRegionMin.x; i < activeRegionMin.x + activeRegionSize.x; ++i) {
        for (int j = activeRegionMin.y; j < activeRegionMin.y + activeRegionSize.y; ++j) {
            for (int k = activeRegionMin.z; k < activeRegionMin.z + activeRegionSize.z; ++k) {
                const auto position = (BLOCK_SIDE_SCALE * CHUNK_SIDE_BLOCK_COUNT) * glm::dvec3(i, j, k);

                const auto relPosition = position - state.playerPosition;
                const BoundingBox chunkBB(relPosition, glm::dvec3(CHUNK_SIDE_SCALE));

                if (chunkBB.isOnFrustrum(state.frustrum)) {
                    const auto mvp = state.viewProjection * glm::translate(glm::mat4(1.f), glm::vec3(relPosition));
                    shader->setMat4("mvp", mvp);

                    getMesh(glm::ivec3(i, j, k)).draw();
                }
            }
        }
    }

    glUseProgram(0);
    Texture2dArray::unbind();
}
