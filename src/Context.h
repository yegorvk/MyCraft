//
// Created by egorv on 4/13/2023.
//

#ifndef SHITCRAFT_CONTEXT_H
#define SHITCRAFT_CONTEXT_H

#include <memory>
#include "asset/AssetManager.h"

class Context {
public:
    static Context &global();

    static void setGlobal(Context *context);

    static void destroyGlobal();

    inline Context() : assetLoader(std::make_unique<DefaultAssetLoader>()), assetManager(*assetLoader) {}

    inline AssetProvider &getAssets() {
        return assetManager;
    }
private:
    std::unique_ptr<AssetLoader> assetLoader;
    AssetManager assetManager;
};

#endif //SHITCRAFT_CONTEXT_H
