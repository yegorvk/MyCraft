//
// Created by egorv on 4/16/2023.
//

#ifndef SHITCRAFT_ASSETREGISTRY_H
#define SHITCRAFT_ASSETREGISTRY_H

#include <string>
#include <unordered_map>
#include "Asset.h"
#include "AssetReader.h"

class AssetRegistry {
public:
    AssetRegistry(AssetReader &reader, const char *rootMap);
private:
    std::unordered_map<std::string, Asset> assets;
};


#endif //SHITCRAFT_ASSETREGISTRY_H
