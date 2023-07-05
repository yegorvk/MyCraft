//
// Created by egorv on 4/13/2023.
//

#ifndef SHITCRAFT_GAMECONTEXT_H
#define SHITCRAFT_GAMECONTEXT_H

#include <memory>

#include "BS_thread_pool.hpp"

#include "asset/Manager.h"
#include "asset/LocalFileReader.h"
#include "registry/BlockRegistry.h"

class GameContext {
public:
    static GameContext &global();

    static void setGlobal(GameContext *context);

    static void destroyGlobal();

    GameContext();

    inline asset::Provider &getAssets() {
        return assetManager;
    }

    [[nodiscard]] inline const BlockRegistry &getBlockRegistry() const {
        return blockRegistry;
    }

private:
    asset::Manager assetManager;
    BlockRegistry blockRegistry;
};

#endif //SHITCRAFT_GAMECONTEXT_H
