//
// Created by egorv on 4/26/2023.
//

#include <stack>

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

    std::stack<std::pair<glm::ivec3, glm::ivec3>> visibleRegions;
    visibleRegions.emplace(activeRegionMin, activeRegionSize);

    while (!visibleRegions.empty()) {
        auto [minChunk, sizeInChunks] = std::move(visibleRegions.top());
        visibleRegions.pop();

        if (sizeInChunks.x == 0 || sizeInChunks.y == 0 || sizeInChunks.z == 0)
            continue;

        auto min = BLOCK_SIDE_SCALE * glm::dvec3(minChunk * CHUNK_SIDE_BLOCK_COUNT);
        auto size = BLOCK_SIDE_SCALE * glm::dvec3(sizeInChunks * CHUNK_SIDE_BLOCK_COUNT);

        auto minRel = min - state.playerPosition;

        BoundingBox chunkBB(minRel, size);

        if (chunkBB.isOnFrustrum(state.frustrum)) {
            if (sizeInChunks == glm::ivec3(1)) {
                auto mvp = state.viewProjection;
                mvp *= glm::translate(glm::mat4(1.f), glm::vec3(minRel));

                shader->setMat4("mvp", mvp);

                getMesh(minChunk).draw();
            } else {
                if (sizeInChunks.x >= sizeInChunks.y && sizeInChunks.x >= sizeInChunks.z) {
                    auto firstHalfSize = glm::ivec3(sizeInChunks.x / 2, sizeInChunks.y, sizeInChunks.z);
                    auto secondHalfSize = glm::ivec3(sizeInChunks.x - sizeInChunks.x / 2, sizeInChunks.y, sizeInChunks.z);

                    visibleRegions.emplace(minChunk, firstHalfSize);
                    visibleRegions.emplace(minChunk + glm::ivec3(firstHalfSize.x, 0, 0), secondHalfSize);
                } else if (sizeInChunks.y >= sizeInChunks.x && sizeInChunks.y >= sizeInChunks.z) {
                    auto firstHalfSize = glm::ivec3(sizeInChunks.x, sizeInChunks.y / 2, sizeInChunks.z);
                    auto secondHalfSize = glm::ivec3(sizeInChunks.x, sizeInChunks.y - sizeInChunks.y / 2, sizeInChunks.z);

                    visibleRegions.emplace(minChunk, firstHalfSize);
                    visibleRegions.emplace(minChunk + glm::ivec3(0, firstHalfSize.y, 0), secondHalfSize);
                } else {
                    auto firstHalfSize = glm::ivec3(sizeInChunks.x, sizeInChunks.y, sizeInChunks.z / 2);
                    auto secondHalfSize = glm::ivec3(sizeInChunks.x, sizeInChunks.y, sizeInChunks.z - sizeInChunks.z / 2);

                    visibleRegions.emplace(minChunk, firstHalfSize);
                    visibleRegions.emplace(minChunk + glm::ivec3(0, 0, firstHalfSize.z), secondHalfSize);
                }
            }
        }
    }

//    for (int i = 0; i < activeRegionSize.x; ++i) {
//        for (int j = 0; j < activeRegionSize.y; ++j) {
//            for (int k = 0; k < activeRegionSize.z; ++k) {
//                const auto chunkPos = activeRegionMin + glm::ivec3(i, j, k);
//                const auto pos = BLOCK_SIDE_SCALE * glm::dvec3(CHUNK_SIDE_BLOCK_COUNT * chunkPos);
//                const auto relPos = pos - state.playerPosition;
//
//                const BoundingBox chunkBB(relPos, glm::dvec3(CHUNK_SIDE_SCALE));
//
//                if (chunkBB.isOnFrustrum(state.frustrum)) {
//                    auto mvp = state.viewProjection;
//                    mvp *= glm::translate(glm::mat4(1.f), glm::vec3(relPos));
//
//                    shader->setMat4("mvp", mvp);
//
//                    getMesh(chunkPos).draw();
//                }
//            }
//        }
//    }

    glUseProgram(0);
    Texture2dArray::unbind();
}
