//
// Created by egorv on 4/10/2023.
//

#include "Texture.h"

Texture Texture::texture2D(const Image &data, TextureOptions options) {
    uint handle = 0;
    glGenTextures(1, &handle);

    glBindTexture(GL_TEXTURE_2D, handle);

    applyTextureOptions(GL_TEXTURE_2D, options);

    glTexImage2D(GL_TEXTURE_2D,
                 0, // mipmap level
                 static_cast<GLint>(data.getFormat()), // gpu pixel format
                 data.getWidth(),
                 data.getHeight(),
                 0, // border
                 static_cast<GLint>(data.getFormat()), // input pixels format
                 GL_UNSIGNED_BYTE,
                 data.getPixels());

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture(handle, TextureType::Tex2D);
}

void Texture::applyTextureOptions(GLenum target, TextureOptions options) {
    glTexParameteri(target, GL_TEXTURE_WRAP_S, static_cast<int>(options.wrapping));
    glTexParameteri(target, GL_TEXTURE_WRAP_T, static_cast<int>(options.wrapping));
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<int>(options.minFilter));
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<int>(options.magFilter));
}