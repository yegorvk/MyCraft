//
// Created by egorv on 4/10/2023.
//

#include <utility>

#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "glad/glad.h"

#include "Texture.h"

static void applyTextureOptions(GLenum target, TexSamplerOptions options);

constexpr int getTextureFormat(int channelCount) {
    switch (channelCount) {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            return 0;
    }
}

constexpr int getTextureInternalFormat(int channelCount) {
    switch (channelCount) {
        case 1:
            return GL_R8;
        case 2:
            return GL_RG8;
        case 3:
            return GL_RGB8;
        case 4:
            return GL_RGBA8;
        default:
            return 0;
    }
}

Texture2d TextureFactory::texture2d(const Image &image, TexSamplerOptions options) {
    auto texture = Texture2d::create();
    texture.bind();

    applyTextureOptions(GL_TEXTURE_2D, options);

    glTexImage2D(GL_TEXTURE_2D,
                 0, // mipmap level
                 getTextureInternalFormat(image.getChannelCount()), // gpu pixel description
                 image.getWidth(),
                 image.getHeight(),
                 0, // border
                 getTextureFormat(image.getChannelCount()), // input pixels description
                 GL_UNSIGNED_BYTE,
                 image.getPixels());

    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

static void applyTextureOptions(GLenum target, TexSamplerOptions options) {
    glTexParameteri(target, GL_TEXTURE_WRAP_S, static_cast<int>(options.wrapMode));
    glTexParameteri(target, GL_TEXTURE_WRAP_T, static_cast<int>(options.wrapMode));
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<int>(options.minFilter));
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<int>(options.magFilter));

    glTexParameterf(target, GL_TEXTURE_LOD_BIAS, options.lodBias);

    if (options.maxAnisotropy != NO_ANISOTROPY)
        glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, options.maxAnisotropy);
}

detail::Texture2dArrayBuilder::Texture2dArrayBuilder(Tex2dDesc format, int layerCount, TexSamplerOptions options)
        : description(format), layerCount(layerCount) {
    tex = Texture2dArray::create();
    tex.bind();

    applyTextureOptions(GL_TEXTURE_2D_ARRAY, options);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, format.width, format.height, layerCount, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
}

void detail::Texture2dArrayBuilder::setLayer(int layer, const Image &image) {
    if (layer < 0 || layer >= layerCount)
        throw std::invalid_argument("Layer index must be non-negative and less than" + std::to_string(layerCount));
    if (image.getChannelCount() != description.channelCount) {
        spdlog::error("Could not set array registry layer pixels: incompatible pixel formats");
        throw std::invalid_argument("could not set layer pixels because of incompatible registry formats");
    } else if (image.getWidth() != description.width || image.getHeight() != description.height) {
        spdlog::warn("Dimensions of provided array layer image differ from ones of the registry");
        setLayer(layer, image.resize(description.width, description.height));
    } else {
        tex.bind();

        auto format = getTextureFormat(description.channelCount);

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, description.width, description.height, 1, format,
                        GL_UNSIGNED_BYTE,
                        image.getPixels());
    }
}

Texture2dArray detail::Texture2dArrayBuilder::build() {
    tex.bind();
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    return std::move(tex);
}