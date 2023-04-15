//
// Created by egorv on 4/10/2023.
//

#include <utility>
#include "renderer/Texture.h"

Texture2D Texture2D::loadFromMemory(int width,
                                    int height,
                                    PixelFormat format,
                                    const uint8_t *pixels,
                                    TexSamplerOptions options) {
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

    return std::move(Texture2D(handle)); // NOLINT(modernize-return-braced-init-list)
}
