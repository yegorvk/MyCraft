//
// Created by egorv on 4/26/2023.
//

#include "spdlog/spdlog.h"

#include "ViewFrustrum.h"

#include "Context.h"
#include "BlocksRenderer.h"

constexpr float BLOCK_SIDE_LEN = 0.2f;

BlocksRenderer::BlocksRenderer(World &world)
        : chunksStateChangedListener([this](const auto &event) {
              chunksStateChanged(event.region, event.newState);
          }),
          reloadChunksListener([this](const auto &event) {
              reloadChunks(event.loadedRegion);
          }),
          world(world),
          shader(Context::global().getAssets().getShader("@shader/chunk")),
          texture(createArrayTexture()) {
    id = reinterpret_cast<std::size_t>(this);

    world.getEventManager().addEventListener(id, chunksStateChangedListener);
    world.getEventManager().addEventListener(id, reloadChunksListener);
}

BlocksRenderer::~BlocksRenderer() {
    //world.getEventManager().removeEventListener<ChunksStateChanged>(id);
    //world.getEventManager().removeEventListener<ReloadChunks>(id);
}

void BlocksRenderer::reloadChunks(AAB loadedRegion) {
    chunkMeshes.reset(loadedRegion.getMax() - loadedRegion.getMin() + glm::ivec3(1));

    world.forEachLoadedChunk([this](glm::ivec3 position, const Chunk &chunk) {
        updateChunkMesh(position, chunk);
    });
}

void BlocksRenderer::chunksStateChanged(AAB region, ChunkState newState) {
    if (newState == ChunkState::Released)
        return;

    world.forEachChunk([this](glm::ivec3 position, const Chunk &chunk) {
        updateChunkMesh(position, chunk);
    }, region);
}

void BlocksRenderer::updateChunkMesh(glm::ivec3 position, const Chunk &chunk) {
    auto &mesh = chunkMeshes.get(position);
    const auto offset = glm::vec3(position << chunk.getSideBlockCountLog2()) * BLOCK_SIDE_LEN;

    mesh.setTilesTexture(texture);
    mesh.update(world.getBlocks(), chunk, BLOCK_SIDE_LEN, offset);
}

void BlocksRenderer::draw(const Transform &transform) const {
    shader.bind();
    shader.setMat4("mvp", transform.mvp);

    BoundingBox box(glm::vec3(0.f), glm::vec3(16.f * 0.2f));

    const glm::vec3 chunkExtents(BLOCK_SIDE_LEN * static_cast<float>(1 << CHUNK_SIDE_BLOCK_COUNT_LOG2) / 2.f);

    chunkMeshes.forEach([chunkExtents, transform](glm::ivec3 position, const ChunkMesh &mesh) {
        BoundingBox chunkBB(chunkExtents * (glm::vec3(position) * 2.f + 1.f), chunkExtents);

        if (chunkBB.isOnFrustrum(transform.frustrum))
            mesh.draw();
    }, world.getLoadedRegion());
}

Texture BlocksRenderer::createArrayTexture() {
    const auto &image = Context::global().getAssets().getImage("@img/grass_top");
    std::array<std::reference_wrapper<const Image>, 1> images = {{std::cref(image)}};

    return Texture::texture2dArray(image.getWidth(), image.getHeight(), image.getFormat(), images.begin(), images.end());
}
