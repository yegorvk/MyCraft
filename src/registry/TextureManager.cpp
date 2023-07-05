//
// Created by egorv on 6/3/2023.
//

#include "TextureManager.h"

void TextureManager::registerBlock(const asset::BlockAsset &blockAsset, const asset::Provider &provider) {
    for (int i = 0; i < 6; ++i) {
        const auto &face = blockAsset.getFace(i);

        addImage(face.getTextureRef(), provider.getImage(face.getTextureRef()));
    }
}

TextureId TextureManager::addImage(const std::string &key, const Image &image) {
    if (contains(key))
        return getId(key);

    images.emplace_back(&image);

    auto id = static_cast<TextureId>(images.size());
    registerTexture(key, id);

    return id;
}

Texture TextureManager::createArrayTexture(TextureDescription description, TextureOptions options) const {
    if (images.empty())
        return {};

    const auto &firstImage = **images.begin();

    description = TextureDescription(firstImage.getWidth(), firstImage.getHeight(), firstImage.getChannelCount())
            .override(description);

    std::vector<std::reference_wrapper<const Image>> imageList;
    imageList.reserve(images.size());

    for (const auto &image: images)
        imageList.emplace_back(*image);

    return Texture::texture2dArray(description, imageList.begin(), imageList.end(), options);
}
