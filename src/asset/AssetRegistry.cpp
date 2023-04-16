//
// Created by egorv on 4/16/2023.
//

#include "AssetRegistry.h"
#include "json.hpp"

using json = nlohmann::json;

AssetRegistry::AssetRegistry(AssetReader &reader, const char *rootMap) {
    auto root = json::parse(reader.getText(rootMap));


}
