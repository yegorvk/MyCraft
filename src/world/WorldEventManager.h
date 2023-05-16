//
// Created by egorv on 5/7/2023.
//

#ifndef SHITCRAFT_WORLDEVENTMANAGER_H
#define SHITCRAFT_WORLDEVENTMANAGER_H

#include <map>
#include <functional>

#include "glm/glm.hpp"

#include "TypedContainer.h"
#include "AAB.h"

enum class ChunkState {
    Loaded,
    Released
};

struct ChunksStateChanged;

struct ReloadChunks;

template<typename T>
struct WorldEvent {
    WorldEvent() = delete;
};

template<>
struct WorldEvent<ChunksStateChanged> {
    constexpr WorldEvent(AAB region, ChunkState newState) : region(region), newState(newState) {}

    AAB region;
    ChunkState newState;
};

template<>
struct WorldEvent<ReloadChunks> {
    constexpr explicit WorldEvent(AAB loadedRegion) : loadedRegion(loadedRegion) {}

    AAB loadedRegion;
};

template<typename Event>
using WorldEventListener = std::function<void(const WorldEvent<Event> &)>;

namespace detail {
    template<typename K>
    struct map_type {
        template<typename V>
        using type = std::map<K, V>;
    };

    template<typename K, typename ...Events>
    using WorldEventListeners = TypedContainer<map_type<K>, WorldEventListener <Events>...>;
}

template<typename K>
class AbstractWorldEventManager {
public:
    template<typename T>
    inline void addEventListener(K key, WorldEventListener<T> listener) {
        eventListeners.template get<WorldEventListener<T>>().emplace(std::move(key), std::move(listener));
    }

    template<typename T>
    inline void removeEventListener(const K &key) {
        eventListeners.template get<WorldEventListener<T>>().erase(key);
    }

    template<typename T>
    inline void dispatchEvent(const WorldEvent<T> &event) {
        for (auto &entry: eventListeners.template get<WorldEventListener<T>>())
            (entry.second)(event);
    }

private:
    detail::WorldEventListeners<K, ChunksStateChanged, ReloadChunks> eventListeners;
};


using WorldEventManager = AbstractWorldEventManager<std::size_t>;

#endif //SHITCRAFT_WORLDEVENTMANAGER_H
