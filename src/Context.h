//
// Created by egorv on 4/13/2023.
//

#ifndef SHITCRAFT_CONTEXT_H
#define SHITCRAFT_CONTEXT_H

#include <memory>
#include "asset/Manager.h"
#include "asset/LocalFileReader.h"

class Context {
public:
    static Context &global();

    static void setGlobal(Context *context);

    static void destroyGlobal();

    inline Context() : assetManager(asset::Manager::createDefault(std::make_unique<asset::LocalFileReader>())) {}

    inline asset::Provider &getAssets() {
        return assetManager;
    }

private:
    asset::Manager assetManager;
};

#endif //SHITCRAFT_CONTEXT_H
