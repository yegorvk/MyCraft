//
// Created by egorv on 5/7/2023.
//

#ifndef SHITCRAFT_WORLD_H
#define SHITCRAFT_WORLD_H

#include <queue>
#include <optional>

#include "block/BlockRegistry.h"
#include "WorldGenerator.h"
#include "ArrayUtils.h"
#include "MathUtils.h"

struct ChunkUpdateRequest {
    glm::ivec3 position;
    ChunkData *chunk;
};

struct ChunkMeshUpdateRequest {
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

    std::optional<ChunkMeshUpdateRequest> dequeueMeshUpdateRequest();

private:
    inline ChunkData &getChunkData(glm::ivec3 position) {
        return chunks[flatten(positiveMod(position, activeRegionSize), activeRegionSize)];
    }

    [[nodiscard]] static inline glm::ivec3 getChunkPosition(glm::dvec3 position) {
        return {glm::floor(position / CHUNK_SIDE_SCALE)};
    }

    void scheduleLoadChunk(glm::ivec3 position);

    void scheduleLoadChunks(glm::ivec3 min, glm::ivec3 max);

    void loadChunk(const ChunkUpdateRequest &req);

    void shiftLoadedRegion(glm::ivec3 delta);

    glm::ivec3 activeRegionMin{}, activeRegionSize{}, playerChunk{};

    std::vector<ChunkData> chunks;

    std::queue<ChunkUpdateRequest> loadChunkRequests;
    std::queue<ChunkMeshUpdateRequest> chunkMeshUpdateRequests;

    WorldGenerator generator;
};

#endif //SHITCRAFT_WORLD_H
