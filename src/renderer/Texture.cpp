//
// Created by egorv on 4/10/2023.
//

#include <utility>

#include "spdlog/spdlog.h"
#include "Texture.h"
#include "stb_image.h"

Texture Texture::loadTex2dFromBytes(const unsigned char *bytes, std::size_t size, TexOptions options) {
    int width, height, channels;
    auto pixels = stbi_load_from_memory(bytes, static_cast<int>(size), &width, &height, &channels, 0);

    PixelFormat format;

    switch (channels) {
        case 1:
            format = PixelFormat::Grayscale;
            break;
        case 3:
            format = PixelFormat::Rgb;
            break;
        case 4:
            format = PixelFormat::Rgba;
            break;
        default:
            SPDLOG_ERROR("Unsupported number of texture color channels: {}", channels);
            stbi_image_free(pixels);
            return Texture(0, TexType::Tex2D);
    }

    auto texture = Texture::loadTex2dFromMemory(width, height, format, pixels, options);

    stbi_image_free(pixels);

    return std::move(texture);
}

Texture Texture::loadTex2dFromMemory(int width,
                                     int height,
                                     PixelFormat format,
                                     const uint8_t *pixels,
                                     TexOptions options) {
    uint handle = 0;
    glGenTextures(1, &handle);

    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(options.wrapping));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(options.wrapping));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(options.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(options.magFilter));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, static_cast<GLenum>(format), GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return std::move(Texture(handle, TexType::Tex2D)); // NOLINT(modernize-return-braced-init-list)
}