//
// Created by egorv on 5/7/2023.
//

#ifndef SHITCRAFT_WORLD_H
#define SHITCRAFT_WORLD_H

#include "WorldEventManager.h"
#include "Chunk.h"
#include "WrapAround3dArray.h"
#include "BlockCache.h"

class World {
public:
    World() = default;

    void reloadedChunks(AAB region);

    template<typename F>
    inline void forEachLoadedChunk(F &&consumer) {
        forEachChunk(std::forward<F>(consumer), loadedRegion);
    }

    template<typename F>
    inline void forEachChunk(F &&consumer, AAB region) const {
        loadedChunks.forEach(std::forward<F>(consumer), region.intersectWith(loadedRegion));
    }

    [[nodiscard]] inline WorldEventManager &getEventManager() {
        return eventManager;
    }

    [[nodiscard]] inline const WorldEventManager &getEventManager() const {
        return eventManager;
    }

    [[nodiscard]] inline const BlockCache &getBlocks() const {
        return cache;
    }

    [[nodiscard]] inline AAB getLoadedRegion() const {
        return loadedRegion;
    }

private:
    AAB loadedRegion;
    WrapAround3dArray<Chunk> loadedChunks;

    BlockCache cache;

    WorldEventManager eventManager;
};

#endif //SHITCRAFT_WORLD_H
