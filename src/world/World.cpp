//
// Created by egorv on 5/7/2023.
//

#include "spdlog/spdlog.h"

#include "World.h"
#include "utils/MathUtils.h"
#include "GameContext.h"

constexpr int MAX_OPERATIONS_PER_TICK = 10;

void World::setBlockAndQueueUpdate(glm::ivec3 position, BlockId block) {
    auto [chunkPos, localBlockPos] = parseBlockPosition(position);

    if (!contains(chunkPos, activeRegionMin, activeRegionMin + activeRegionSize - 1)) {
        spdlog::warn("Setting block at position ({}; {}; {}) which is outside the loaded region", position.x,
                     position.y, position.z);
        return;
    }

    auto &chunk = getChunkData(chunkPos);

    if (chunk.setBlock(localBlockPos, block)) {
        chunk.updateMesh(GameContext::global().getBlockRegistry());

        // Update neighboring chunks (if necessary)

        // x

        if (localBlockPos.x == 0)
            updateChunkNeighbor(chunkPos, Face::Left);
        else if (localBlockPos.x == CHUNK_SIDE_BLOCK_COUNT - 1)
            updateChunkNeighbor(chunkPos, Face::Right);

        // y

        if (localBlockPos.y == 0)
            updateChunkNeighbor(chunkPos, Face::Bottom);
        else if (localBlockPos.y == CHUNK_SIDE_BLOCK_COUNT - 1)
            updateChunkNeighbor(chunkPos, Face::Top);

        // z

        if (localBlockPos.z == 0)
            updateChunkNeighbor(chunkPos, Face::Back);
        else if (localBlockPos.z == CHUNK_SIDE_BLOCK_COUNT - 1)
            updateChunkNeighbor(chunkPos, Face::Front);

        meshUpdateRequests.emplace(UpdateChunkMeshRequest{chunkPos, &chunk});
    }
}

void World::setTargetBlockAndQueueUpdate(glm::dvec3 camOrigin, glm::dvec3 camDirection, BlockId block) {
    auto targetedBlock = computeTargetBlock(camOrigin, camDirection);

    if (block != 0) {
        auto targetedBlockPoint = rayIntersectAABB(camOrigin, camDirection, targetedBlock);
        int targetedBlockFace = getFaceContainingPointAAB(targetedBlockPoint, targetedBlock, glm::dvec3(1.0));
        targetedBlock += Face::getNormalDirection(targetedBlockFace);
    }

    setBlockAndQueueUpdate(targetedBlock, block);
}

glm::ivec3 World::computeTargetBlock(glm::dvec3 cameraOrigin, glm::dvec3 cameraDirection) const {
    const auto originBlock = getBlockPosition(cameraOrigin);

    // Fast voxel traversal algorithm

    const glm::ivec3 step = glm::sign(cameraDirection);
    const glm::dvec3 tDelta = glm::abs(glm::dvec3(1.0) / cameraDirection);

    glm::dvec3 tMax;

    tMax += glm::dvec3(glm::greaterThan(step, glm::ivec3(0))) * tDelta * (1.0 - glm::fract(cameraOrigin));
    tMax += glm::dvec3(glm::lessThan(step, glm::ivec3(0))) * tDelta * glm::fract(cameraOrigin);

    auto block = originBlock;

    while (length2(block - originBlock) < 10'000) {
        if (getBlock(block) != 0)
            break;

        if (tMax.x < tMax.y && tMax.x < tMax.z) {
            tMax.x += tDelta.x;
            block.x += step.x;
        } else if (tMax.y < tMax.x && tMax.y < tMax.z) {
            tMax.y += tDelta.y;
            block.y += step.y;
        } else {
            tMax.z += tDelta.z;
            block.z += step.z;
        }
    }

    return block;
}

void World::setActiveRegion(glm::ivec3 min, glm::ivec3 size) {
    activeRegionMin = min, activeRegionSize = size;

    chunks.clear();
    chunks.resize(activeRegionSize.x * activeRegionSize.y * activeRegionSize.z);

    scheduleLoadChunks(min, min + size - 1);
}

void World::processPlayerPositionChange(glm::vec3 newPlayerPosition) {
    auto delta = getChunkPosition(newPlayerPosition) - playerChunk;
    setPlayerPosition(newPlayerPosition);

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

void World::updateChunkNeighbor(glm::ivec3 chunkPos, int face) {
    const auto &chunk = getChunkData(chunkPos);
    auto adjChunkPos = chunkPos + Face::getNormalDirection(face);

    if (contains(adjChunkPos, activeRegionMin, activeRegionMin + activeRegionSize - 1)) {
        auto &adjChunk = getChunkData(adjChunkPos);

        adjChunk.updateNeighborData(Face::opposite(face), &chunk);
        adjChunk.updateMesh(GameContext::global().getBlockRegistry());

        meshUpdateRequests.emplace(UpdateChunkMeshRequest{adjChunkPos, &adjChunk});
    }
}