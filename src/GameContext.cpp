//
// Created by egorv on 4/14/2023.
//

#include "GameContext.h"
#include "asset/Parser.h"
#include "asset/CmrcFileReader.h"

GameContext *GLOBAL_CONTEXT = nullptr;

GameContext &GameContext::global() {
    return *GLOBAL_CONTEXT;
}

void GameContext::setGlobal(GameContext *context) {
    GLOBAL_CONTEXT = context;
}

void GameContext::destroyGlobal() {
    delete GLOBAL_CONTEXT;
    GLOBAL_CONTEXT = nullptr;
}

#ifdef USE_CMRC
#define FILE_READER asset::CmrcFileReader
#else
#define FILE_READER asset::LocalFileReader
#endif

GameContext::GameContext() : assetManager(asset::Manager::createDefault(std::make_unique<FILE_READER>())) {
    blockRegistry.registerBlock(1, "@block/grass_block", assetManager);
    blockRegistry.registerBlock(2, "@block/dirt", assetManager);
    blockRegistry.registerBlock(3, "@block/stone", assetManager);
}