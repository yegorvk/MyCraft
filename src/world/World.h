//
// Created by egorv on 5/7/2023.
//

#ifndef SHITCRAFT_WORLD_H
#define SHITCRAFT_WORLD_H

#include <queue>
#include <optional>
#include "DeferredTaskQueue.h"
#include "readerwriterqueue.h"
#include "block/BlockRegistry.h"
#include "WorldGenerator.h"
#include "ArrayUtils.h"
#include "MathUtils.h"

struct LoadChunkRequest {
    glm::ivec3 position;
    ChunkData *chunk;
    int token;
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

    [[nodiscard]] inline glm::ivec3 getActiveRegionMin() const {
        return activeRegionMin;
    }

    [[nodiscard]] inline glm::ivec3 getActiveRegionSize() const {
        return activeRegionSize;
    }

    inline void setPlayerPosition(glm::dvec3 position) {
        playerChunk = getChunkPosition(position);
    }

    void processPlayerPositionChange(glm::vec3 newPlayerPosition);

    void dispatchChunkLoads();

    std::optional<UpdateChunkMeshRequest> dequeueMeshUpdateRequest();

private:
    inline ChunkData &getChunkData(glm::ivec3 position) {
        return chunks[flatten(positiveMod(position, activeRegionSize), activeRegionSize)];
    }

    [[nodiscard]] static inline glm::ivec3 getChunkPosition(glm::dvec3 position) {
        return {glm::floor(position / CHUNK_SIDE_SCALE)};
    }

    void scheduleLoadChunks(glm::ivec3 min, glm::ivec3 max);

    void loadChunk(const LoadChunkRequest &req);

    void updateChunk(const UpdateChunkRequest &req);

    void shiftLoadedRegion(glm::ivec3 delta);

    static constexpr bool contains(glm::ivec3 p, glm::ivec3 min, glm::ivec3 max) {
        return p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y && p.z >= min.z && p.z <= max.z;
    }

    constexpr bool contains(glm::ivec3 p) {
        return contains(p, activeRegionMin, activeRegionMin + activeRegionSize - 1);
    }

    glm::ivec3 activeRegionMin{}, activeRegionSize{}, playerChunk{};
    std::vector<ChunkData> chunks;

    std::queue<std::queue<LoadChunkRequest>> loadChunkRequests;
    std::queue<std::queue<UpdateChunkRequest>> updateChunkRequests;
    std::queue<UpdateChunkMeshRequest> meshUpdateRequests;

    WorldGenerator generator;
};

#endif //SHITCRAFT_WORLD_H
