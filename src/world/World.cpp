//
// Created by egorv on 5/7/2023.
//

#include "World.h"

void World::reloadedChunks(AAB region) {
    eventManager.dispatchEvent(WorldEvent<ChunksStateChanged>(loadedRegion, ChunkState::Released));

    loadedRegion = region;
    loadedChunks.reset(loadedRegion.getMax() - loadedRegion.getMin() + 1);

    // TODO load chunks from new region

    eventManager.dispatchEvent(WorldEvent<ReloadChunks>(region));
}