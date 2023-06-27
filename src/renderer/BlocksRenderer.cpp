//
// Created by egorv on 4/26/2023.
//

#include "spdlog/spdlog.h"

#include "ViewFrustrum.h"
#include "GameContext.h"
#include "chunk/Constants.h"
#include "BlocksRenderer.h"

BlocksRenderer::BlocksRenderer()
        : shader(&GameContext::global().getAssets().getShader("@shader/solid")),
          texture(GameContext::global().getBlockRegistry().getTextureManager().createArrayTexture(
                  TextureDescription(16, 16, 4))) {}

void BlocksRenderer::reset(glm::ivec3 newActiveRegionMin, glm::ivec3 newActiveRegionSize) {
    activeRegionSize = newActiveRegionSize, activeRegionMin = newActiveRegionMin;

    chunkMeshes.clear();
    chunkMeshes.resize(newActiveRegionSize.x * newActiveRegionSize.y * newActiveRegionSize.z);
}

void BlocksRenderer::update(glm::ivec3 chunkPos, const ChunkMeshData *meshData) {
    auto &mesh = getMesh(chunkPos);

    mesh.update(meshData);
    mesh.setTilesTexture(TextureHandle(texture));
}

void
BlocksRenderer::draw(glm::dvec3 cameraPosition, const glm::mat4 &viewProjection, const ViewFrustrum &frustrum) const {
    shader->bind();

    for (int i = activeRegionMin.x; i < activeRegionMin.x + activeRegionSize.x; ++i) {
        for (int j = activeRegionMin.y; j < activeRegionMin.y + activeRegionSize.y; ++j) {
            for (int k = activeRegionMin.z; k < activeRegionMin.z + activeRegionSize.z; ++k) {
                const auto position = CHUNK_SIDE_SCALE * glm::dvec3(i, j, k);
                const auto relPosition = glm::vec3(position - cameraPosition);

                const BoundingBox chunkBB(position - cameraPosition, glm::dvec3(CHUNK_SIDE_SCALE));

                if (chunkBB.isOnFrustrum(frustrum)) {
                    shader->setMat4("mvp", viewProjection * glm::translate(glm::mat4(1.f), relPosition));
                    getMesh(glm::ivec3(i, j, k)).draw();
                }
            }
        }
    }
}
