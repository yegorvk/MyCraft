//
// Created by egorv on 4/13/2023.
//

#ifndef SHITCRAFT_CONTEXT_H
#define SHITCRAFT_CONTEXT_H

#include <memory>

#include "asset/Manager.h"
#include "asset/LocalFileReader.h"
#include "block/BlockRegistry.h"

class Context {
public:
    static Context &global();

    static void setGlobal(Context *context);

    static void destroyGlobal();

    Context();

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

#endif //SHITCRAFT_CONTEXT_H
