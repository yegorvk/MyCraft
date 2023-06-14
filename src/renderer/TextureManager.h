//
// Created by egorv on 6/3/2023.
//

#ifndef SHITCRAFT_TEXTUREMANAGER_H
#define SHITCRAFT_TEXTUREMANAGER_H

#include <cstdint>
#include <set>

#include "Texture.h"
#include "asset/Image.h"
#include "asset/Asset.h"
#include "asset/Manager.h"

#include "TextureLookup.h"

class TextureManager : private TextureLookup {
public:
    TextureManager() = default;

    using TextureLookup::getId;

    void registerBlock(const asset::BlockAsset &blockAsset, const asset::Provider &provider);

    TextureId addImage(const std::string &key, const Image &image);

    [[nodiscard]] Texture createArrayTexture(TextureDescription description, TextureOptions options = {}) const;

private:
    std::vector<const Image *> images;
};


#endif //SHITCRAFT_TEXTUREMANAGER_H
