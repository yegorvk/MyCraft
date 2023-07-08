//
// Created by egorv on 4/26/2023.
//

#include "spdlog/spdlog.h"

#include "ViewFrustrum.h"
#include "GameContext.h"
#include "chunk/Constants.h"
#include "WorldRenderer.h"

WorldRenderer::WorldRenderer()
        : shader(&GameContext::global().getAssets().getShader("@shader/solid")),
          texture(GameContext::global().getBlockRegistry().getTextureManager().createArrayTexture(
                  Tex2dDesc(16, 16, 4), TexSamplerOptions::linearMipmapNearestRepeat())) {}

void WorldRenderer::reset(glm::ivec3 newActiveRegionMin, glm::ivec3 newActiveRegionSize) {
    activeRegionSize = newActiveRegionSize, activeRegionMin = newActiveRegionMin;

    chunkMeshes.clear();
    chunkMeshes.resize(newActiveRegionSize.x * newActiveRegionSize.y * newActiveRegionSize.z);
}

void WorldRenderer::update(glm::ivec3 chunkPos, const ChunkMeshData *meshData) {
    auto &mesh = getMesh(chunkPos);
    mesh.update(meshData);
}

void
WorldRenderer::draw(glm::dvec3 cameraPosition, const glm::mat4 &viewProjection, const ViewFrustrum &frustrum,
                    glm::vec2) const {
    texture.bind();
    shader->bind();

    for (int i = activeRegionMin.x; i < activeRegionMin.x + activeRegionSize.x; ++i) {
        for (int j = activeRegionMin.y; j < activeRegionMin.y + activeRegionSize.y; ++j) {
            for (int k = activeRegionMin.z; k < activeRegionMin.z + activeRegionSize.z; ++k) {
                const auto position = (BLOCK_SIDE_SCALE * CHUNK_SIDE_BLOCK_COUNT) * glm::dvec3(i, j, k);
                const auto relPosition = glm::vec3(position - cameraPosition);

                const BoundingBox chunkBB(position - cameraPosition, glm::dvec3(CHUNK_SIDE_SCALE));

                if (chunkBB.isOnFrustrum(frustrum)) {
                    shader->setMat4("mvp", viewProjection * glm::translate(glm::mat4(1.f), relPosition));
                    shader->setFloat("blockScale", BLOCK_SIDE_SCALE_F);

                    getMesh(glm::ivec3(i, j, k)).draw();
                }
            }
        }
    }

    glUseProgram(0);
}
