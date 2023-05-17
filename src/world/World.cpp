//
// Created by egorv on 5/7/2023.
//

#include "World.h"

void World::reloadAllChunks(AAB region) {
    eventManager.dispatchEvent(WorldEvent<ChunksStateChanged>(loadedRegion, ChunkState::Released));

    loadedRegion = region;
    loadedChunks.reset(loadedRegion.getMax() - loadedRegion.getMin() + 1);

    loadedChunks.forEach([this](glm::ivec3 position, Chunk &chunk) {
        chunk = generator.generate(position);
    }, loadedRegion);

    eventManager.dispatchEvent(WorldEvent<ReloadChunks>(region));
}

void World::loadChunks(AAB region) {
    loadedChunks.forEach([this](glm::ivec3 position, Chunk &chunk) {
        chunk = generator.generate(position);
    }, loadedRegion);

    eventManager.dispatchEvent(WorldEvent<ChunksStateChanged>(region, ChunkState::Loaded));
}

void World::unloadChunks(AAB region) {
    eventManager.dispatchEvent(WorldEvent<ChunksStateChanged>(region, ChunkState::Released));
}