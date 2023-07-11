//
// Created by egorv on 5/7/2023.
//

#ifndef SHITCRAFT_WORLD_H
#define SHITCRAFT_WORLD_H

#include <queue>
#include <optional>
#include "readerwriterqueue.h"
#include "registry/BlockRegistry.h"
#include "WorldGenerator.h"
#include "utils/ArrayUtils.h"
#include "utils/MathUtils.h"

inline std::pair<glm::ivec3, glm::ivec3> parseBlockPosition(glm::ivec3 pos) {
    const glm::ivec3 chunk = glm::floor(glm::dvec3(pos) / double(CHUNK_SIDE_BLOCK_COUNT));
    const auto localPos = pos - CHUNK_SIDE_BLOCK_COUNT * chunk;
    return {chunk, localPos};
}

struct LoadChunkRequest {
    glm::ivec3 position;
    ChunkData *chunk;
};

struct UpdateChunkRequest {
    glm::ivec3 position{};
    ChunkData *chunk;
};

struct UpdateChunkMeshRequest {
    glm::ivec3 position;
    const ChunkData *chunk;
};

class World {
public:
    World() = default;

    void setActiveRegion(glm::ivec3 min, glm::ivec3 size);

    void setTargetBlockAndQueueUpdate(glm::dvec3 camOrigin, glm::dvec3 camDirection, BlockId block);

    void setBlockAndQueueUpdate(glm::ivec3 position, BlockId block);

    [[nodiscard]] inline BlockId getBlock(glm::ivec3 position) const {
        auto [chunkPos, blockPos] = parseBlockPosition(position);
        return getChunkData(chunkPos).getBlock(blockPos);
    }

    [[nodiscard]] inline glm::ivec3 getActiveRegionMin() const {
        return activeRegionMin;
    }

    [[nodiscard]] inline glm::ivec3 getActiveRegionSize() const {
        return activeRegionSize;
    }

    inline void setPlayerPosition(glm::dvec3 position) {
        playerChunk = getChunkPosition(position);
        playerBlock = getBlockPosition(position) - playerChunk * CHUNK_SIDE_BLOCK_COUNT;
    }

    void processPlayerPositionChange(glm::vec3 newPlayerPosition);

    void dispatchChunkLoads();

    std::optional<UpdateChunkMeshRequest> dequeueMeshUpdateRequest();

private:
    inline ChunkData &getChunkData(glm::ivec3 position) {
        return chunks[flatten(positiveMod(position, activeRegionSize), activeRegionSize)];
    }

    [[nodiscard]] inline const ChunkData &getChunkData(glm::ivec3 position) const {
        return chunks[flatten(positiveMod(position, activeRegionSize), activeRegionSize)];
    }

    [[nodiscard]] static inline glm::ivec3 getChunkPosition(glm::dvec3 position) {
        return {glm::floor(position) / double(CHUNK_SIDE_BLOCK_COUNT)};
    }

    [[nodiscard]] static inline glm::ivec3 getBlockPosition(glm::dvec3 position) {
        return {glm::floor(position)};
    }

    [[nodiscard]] glm::ivec3 computeTargetBlock(glm::dvec3 cameraOrigin, glm::dvec3 cameraDirection) const;

    void scheduleLoadChunks(glm::ivec3 min, glm::ivec3 max);

    void loadChunk(const LoadChunkRequest &req);

    void updateChunk(const UpdateChunkRequest &req);

    void shiftLoadedRegion(glm::ivec3 delta);

    void updateChunkNeighbor(glm::ivec3 chunkPos, int face);

    static constexpr bool contains(glm::ivec3 p, glm::ivec3 min, glm::ivec3 max) {
        return p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y && p.z >= min.z && p.z <= max.z;
    }

    constexpr bool contains(glm::ivec3 p) {
        return contains(p, activeRegionMin, activeRegionMin + activeRegionSize - 1);
    }

    glm::ivec3 activeRegionMin{}, activeRegionSize{}, playerChunk{}, playerBlock{};
    std::vector<ChunkData> chunks;

    std::queue<std::queue<LoadChunkRequest>> loadChunkRequests;
    std::queue<std::queue<UpdateChunkRequest>> updateChunkRequests;
    std::queue<UpdateChunkMeshRequest> meshUpdateRequests;

    WorldGenerator generator;
};

#endif //SHITCRAFT_WORLD_H
