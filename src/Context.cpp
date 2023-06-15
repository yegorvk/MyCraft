//
// Created by egorv on 4/14/2023.
//

#include "Context.h"
#include "asset/Parser.h"
#include "asset/CmrcFileReader.h"

Context *GLOBAL_CONTEXT = nullptr;

Context &Context::global() {
    return *GLOBAL_CONTEXT;
}

void Context::setGlobal(Context *context) {
    GLOBAL_CONTEXT = context;
}

void Context::destroyGlobal() {
    delete GLOBAL_CONTEXT;
    GLOBAL_CONTEXT = nullptr;
}

#ifdef USE_CMRC
#define FILE_READER asset::CmrcFileReader
#else
#define FILE_READER asset::LocalFileReader
#endif

Context::Context() : assetManager(asset::Manager::createDefault(std::make_unique<FILE_READER>())) {
    blockRegistry.registerBlock(1, "@block/grass_block", assetManager);
    blockRegistry.registerBlock(2, "@block/dirt", assetManager);
}