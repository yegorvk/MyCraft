//
// Created by egorv on 4/10/2023.
//

#include <utility>

#include "spdlog/spdlog.h"

#include "Texture.h"

static void applyTextureOptions(GLenum target, TextureOptions options);

Texture Texture::texture2d(const Image &image, TextureOptions options) {
    uint handle = 0;
    glGenTextures(1, &handle);

    glBindTexture(GL_TEXTURE_2D, handle);

    applyTextureOptions(GL_TEXTURE_2D, options);

    glTexImage2D(GL_TEXTURE_2D,
                 0, // mipmap level
                 static_cast<GLint>(image.getFormat()), // gpu pixel format
                 image.getWidth(),
                 image.getHeight(),
                 0, // border
                 static_cast<GLint>(image.getFormat()), // input pixels format
                 GL_UNSIGNED_BYTE,
                 image.getPixels());

    glGenerateMipmap(GL_TEXTURE_2D);

    return {handle, TextureType::Tex2d};
}

Texture::Texture(uint handle, TextureType type) : handle(handle), type(type) {}

Texture::Texture(Texture &&other) noexcept {
    *this = std::move(other);
}

Texture::~Texture() {
    if (handle != 0)
        glDeleteTextures(1, &handle);
}

Texture &Texture::operator=(Texture &&other) noexcept {
    type = other.type;
    handle = other.handle;
    other.handle = 0;
    return *this;
}

void Texture::bind() const {
    glBindTexture(static_cast<GLenum>(type), handle);
}

static void applyTextureOptions(GLenum target, TextureOptions options) {
    glTexParameteri(target, GL_TEXTURE_WRAP_S, static_cast<int>(options.wrapping));
    glTexParameteri(target, GL_TEXTURE_WRAP_T, static_cast<int>(options.wrapping));
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, static_cast<int>(options.minFilter));
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, static_cast<int>(options.magFilter));
}

detail::Texture2dArrayBuilder::Texture2dArrayBuilder(int width, int height, int layerCount, PixelFormat format,
                                                     TextureOptions options) : width(width), height(height),
                                                                               layerCount(layerCount), format(format) {
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D_ARRAY, handle);

    applyTextureOptions(GL_TEXTURE_2D_ARRAY, options);

    auto glFormat = getGlPixelFormat(format);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB8, width, height, layerCount, 0, glFormat, GL_UNSIGNED_BYTE, nullptr);
}

void detail::Texture2dArrayBuilder::setLayer(int layer, const Image &image) {
    if (image.getFormat() != format) {
        spdlog::error("Could not set array texture layer pixels: incompatible pixel formats");
        throw std::invalid_argument("could not set layer pixels because of incompatible texture formats");
    } else if (image.getWidth() != width || image.getHeight() != height) {
        spdlog::warn("Dimensions of provided array layer image differ from ones of the texture");
        setLayer(layer, image.resize(width, height));
    } else {
        glBindTexture(GL_TEXTURE_2D_ARRAY, handle);

        auto glFormat = getGlPixelFormat(format);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, width, height, 1, glFormat, GL_UNSIGNED_BYTE, image.getPixels());
    }
}

Texture detail::Texture2dArrayBuilder::build() {
    glBindTexture(GL_TEXTURE_2D_ARRAY, handle);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    auto texture = Texture(handle, TextureType::Tex2dArray);
    handle = 0;

    return texture;
}
