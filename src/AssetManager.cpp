//
// Created by egorv on 4/12/2023.
//

#include "AssetManager.h"
#include "utils.h"
#include "stb_image.h"
#include "spdlog/spdlog.h"

std::string LocalFsAssetLoader::loadStr(const char *path) {
    return readTextFromFile((rootDir + path).c_str());
}

Texture2D LocalFsAssetLoader::loadTex2D(const char *path, TexSamplerOptions options) {
    auto data = readBytesFromFile((rootDir + path).c_str());

    int width, height, nChannels;
    auto img = stbi_load_from_memory(data.data(), static_cast<int>(data.size()), &width, &height, &nChannels, 0);

    PixelFormat format;

    switch (nChannels) {
        case 1:
            format = PixelFormat::Grayscale;
            break;
        case 3:
            format = PixelFormat::Rgb;
            break;
        case 4:
            format = PixelFormat::Rgba;
            break;
        default: {
            spdlog::error("Invalid number of channels in \"{}\": {}", path, nChannels);
            return {};
        }
    }

    auto tex = Texture2D::loadFromMemory(width, height, format, img, options);
    stbi_image_free(img);

    return tex;
}
