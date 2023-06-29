//
// Created by egorv on 5/7/2023.
//

#include "spdlog/spdlog.h"
#include "World.h"
#include "GameContext.h"

constexpr int MAX_OPERATIONS_PER_TICK = 10;

void World::setActiveRegion(glm::ivec3 min, glm::ivec3 size) {
    activeRegionMin = min, activeRegionSize = size;

    chunks.clear();
    chunks.resize(activeRegionSize.x * activeRegionSize.y * activeRegionSize.z);

    scheduleLoadChunks(min, min + size - 1);
}

void World::processPlayerPositionChange(glm::vec3 newPlayerPosition) {
    auto delta = getChunkPosition(newPlayerPosition) - playerChunk;
    playerChunk += delta;

    shiftLoadedRegion(delta);
}

void World::dispatchChunkLoads() {
    for (int i = 0;
         i < MAX_OPERATIONS_PER_TICK && !loadChunkRequests.empty() && !loadChunkRequests.front().empty(); ++i) {
        const auto req = loadChunkRequests.front().front();
        loadChunkRequests.front().pop();
        loadChunk(req);
    }

    if (loadChunkRequests.empty() || loadChunkRequests.front().empty()) {
        for (int i = 0; i < MAX_OPERATIONS_PER_TICK && !updateChunkRequests.empty() &&
                        !updateChunkRequests.front().empty(); ++i) {
            const auto req = updateChunkRequests.front().front();
            updateChunkRequests.front().pop();
            updateChunk(req);
        }

        if (!updateChunkRequests.empty() && updateChunkRequests.front().empty()) {
            loadChunkRequests.pop();
            updateChunkRequests.pop();
        }
    }
}

std::optional<UpdateChunkMeshRequest> World::dequeueMeshUpdateRequest() {
    if (meshUpdateRequests.empty())
        return std::nullopt;
    else {
        auto req = meshUpdateRequests.front();
        meshUpdateRequests.pop();
        return req;
    }
}

void World::scheduleLoadChunks(glm::ivec3 min, glm::ivec3 max) {
    loadChunkRequests.emplace();
    updateChunkRequests.emplace();

    for (int x = min.x; x <= max.x; ++x) {
        for (int y = min.y; y <= max.y; ++y) {
            for (int z = min.z; z <= max.z; ++z) {
                glm::ivec3 pos(x, y, z);
                auto &chunk = getChunkData(pos);

                meshUpdateRequests.push(UpdateChunkMeshRequest{pos, nullptr});

                loadChunkRequests.back().push(LoadChunkRequest{pos, &chunk});
                updateChunkRequests.back().push(UpdateChunkRequest{pos, &chunk});

                for (int i = 0; i < 6; ++i) {
                    auto adjPos = pos + Face::getNormalDirection(i);

                    if (!contains(adjPos, min, max) && contains(adjPos))
                        updateChunkRequests.back().push(UpdateChunkRequest{adjPos, &getChunkData(adjPos)});
                }
            }
        }
    }
}

void World::loadChunk(const LoadChunkRequest &req) {
    auto &chunk = getChunkData(req.position);
    chunk = generator.generate(req.position);
}

void World::updateChunk(const UpdateChunkRequest &req) {
    for (int i = 0; i < 6; ++i) {
        auto adjPos = req.position + Face::getNormalDirection(i);

        if (contains(adjPos))
            req.chunk->updateNeighborData(i, &getChunkData(adjPos));
        else if (contains(req.position))
            req.chunk->updateNeighborData(i, nullptr);
    }

    req.chunk->updateMesh(GameContext::global().getBlockRegistry());
    meshUpdateRequests.push(UpdateChunkMeshRequest{req.position, req.chunk});
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