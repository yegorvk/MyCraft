//
// Created by egorv on 5/7/2023.
//

#include "World.h"
#include "chunk/Constants.h"
#include "spdlog/spdlog.h"
#include "Context.h"

constexpr int MAX_CHUNK_LOADS_PER_TICK = 20;

void World::setActiveRegion(glm::ivec3 min, glm::ivec3 size) {
    activeRegionMin = min, activeRegionSize = size;

    chunks.clear();
    chunks.resize(activeRegionSize.x * activeRegionSize.y * activeRegionSize.z);

    for (int x = min.x; x < min.x + size.x; ++x) {
        for (int y = min.y; y < min.y + size.y; ++y) {
            for (int z = min.z; z < min.z + size.z; ++z) {
                const glm::ivec3 position(x, y, z);
                loadChunk(ChunkUpdateRequest{position, &getChunkData(position)});
            }
        }
    }
}

void World::processPlayerPositionChange(glm::vec3 newPlayerPosition) {
    auto delta = getChunkPosition(newPlayerPosition) - playerChunk;
    playerChunk += delta;

    shiftLoadedRegion(delta);
}

void World::dispatchChunkLoads() {
    for (int i = 0; i < MAX_CHUNK_LOADS_PER_TICK && !loadChunkRequests.empty(); ++i) {
        const auto req = loadChunkRequests.front();
        loadChunkRequests.pop();

        loadChunk(req);
    }
}

std::optional<ChunkMeshUpdateRequest> World::dequeueMeshUpdateRequest() {
    if (!chunkMeshUpdateRequests.empty()) {
        const auto req = chunkMeshUpdateRequests.front();
        chunkMeshUpdateRequests.pop();

        return req;
    } else
        return std::nullopt;
}

void World::scheduleLoadChunk(glm::ivec3 position) {
    auto &chunk = getChunkData(position);
    const auto req = ChunkUpdateRequest{position, &chunk};

    loadChunkRequests.push(req);
}

void World::scheduleLoadChunks(glm::ivec3 min, glm::ivec3 max) {
    for (int x = min.x; x <= max.x; ++x)
        for (int y = min.y; y <= max.y; ++y)
            for (int z = min.z; z <= max.z; ++z)
                scheduleLoadChunk(glm::ivec3(x, y, z));
}

void World::loadChunk(const ChunkUpdateRequest &req) {
    auto &chunk = getChunkData(req.position);

    chunk = generator.generate(req.position);
    chunk.updateMesh(Context::global().getBlockRegistry());

    chunkMeshUpdateRequests.push(ChunkMeshUpdateRequest{req.position, &chunk});
}

void World::shiftLoadedRegion(glm::ivec3 delta) {
    auto min = activeRegionMin, max = activeRegionMin + activeRegionSize - 1;

    if (delta.x > 0)
        scheduleLoadChunks({max.x + 1, min.y, min.z}, {max.x + delta.x, max.y, max.z});
    else if (delta.x < 0)
        scheduleLoadChunks({min.x + delta.x, min.y, min.z}, {min.x - 1, max.y, max.z});

    activeRegionMin.x += delta.x;
    min.x += delta.x, max.x += delta.x;

    if (delta.z > 0)
        scheduleLoadChunks({min.x, min.y, max.z + 1}, {max.x, max.y, max.z + delta.z});
    else if (delta.z < 0)
        scheduleLoadChunks({min.x, min.y, min.z + delta.z}, {max.x, max.y, min.z - 1});

    activeRegionMin.z += delta.z;
    min.z += delta.z, max.z += delta.z;

//    if (delta.y > 0)
//        scheduleLoadChunks({min.x, max.y + 1, min.z}, {max.x, max.y + delta.y, max.z});
//    else if (delta.y < 0)
//        scheduleLoadChunks({min.x, min.y + delta.y, min.z}, {max.x, min.y - 1, max.z});
//
//    activeRegionMin.y += delta.y;
}