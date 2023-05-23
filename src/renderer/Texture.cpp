//
// Created by egorv on 4/10/2023.
//

#include <utility>

#include "spdlog/spdlog.h"
#include "glm/glm.hpp"

#include "Texture.h"

inline glm::u16vec4 scaleDown(glm::u8vec4 p1, glm::u8vec4 p2, glm::u8vec4 p3, glm::u8vec4 p4) {
    const auto p1f = glm::vec4(p1) / 255.f;
    const auto p2f = glm::vec4(p2) / 255.f;
    const auto p3f = glm::vec4(p3) / 255.f;
    const auto p4f = glm::vec4(p4) / 255.f;

    const auto colorF = glm::sqrt((p1f * p1f + p2f * p2f + p3f * p3f + p4f * p4f) / 4.f);
    return glm::u8vec4(glm::clamp(colorF * 255.f, 0.f, 255.f));
}

constexpr int NUM_MIPMAP_LEVELS = 3;

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

Texture::Texture(uint handle, TextureType type) : type(type), handle(handle) {}

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

    float maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

    glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
}

detail::Texture2dArrayBuilder::Texture2dArrayBuilder(int width, int height, int layerCount, PixelFormat format,
                                                     TextureOptions options) : width(width), height(height),
                                                                               layerCount(layerCount), format(format) {
    if (format != PixelFormat::Rgba) {
        spdlog::error("Unsupported pixel format. Use rgba8.");
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D_ARRAY, handle);

    applyTextureOptions(GL_TEXTURE_2D_ARRAY, options);

    auto glFormat = getGlPixelFormat(format);

    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, NUM_MIPMAP_LEVELS - 1);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layerCount, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);

//    for (int level = 0; level < NUM_MIPMAP_LEVELS; ++level)
//        glTexImage3D(GL_TEXTURE_2D_ARRAY, level, GL_RGBA8, width >> level, height >> level, layerCount, 0, GL_RGBA,
//                     GL_UNSIGNED_BYTE, nullptr);
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

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, width, height, 1, glFormat, GL_UNSIGNED_BYTE,
                        image.getPixels());

//        std::vector<glm::u8vec4> pixels(image.getWidth() * image.getHeight());
//        memcpy(pixels.data(), image.getPixels(), pixels.size() * sizeof(glm::u8vec4));
//
//        std::vector<glm::u8vec4> temp(pixels.size());
//
//        for (int level = 1; level < NUM_MIPMAP_LEVELS; ++level) {
//            const int levelWidth = width >> level, levelHeight = height >> level;
//            const int prevLevelWidth = levelWidth * 2;
//
//            for (int i = 0; i < levelHeight; ++i) {
//                for (int j = 0; j < levelWidth; ++j) {
//                    const int i1 = 2 * i, j1 = j * 2;
//
//                    const auto p1 = pixels[i1 * prevLevelWidth + j1];
//                    const auto p2 = pixels[i1 * prevLevelWidth + j1 + 1];
//                    const auto p3 = pixels[(i1 + 1) * prevLevelWidth + j1];
//                    const auto p4 = pixels[(i1 + 1) * prevLevelWidth + j1 + 1];
//
//                    temp[i * levelWidth + j] = scaleDown(p1, p2, p3, p4);
//                    temp[i * levelWidth + j].a = 0xFF;
//                }
//            }
//
//            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, layer, levelWidth, levelHeight, 1, GL_RGBA,
//                            GL_UNSIGNED_BYTE,
//                            temp.data());
//
//            std::swap(pixels, temp);
//        }
    }
}

Texture detail::Texture2dArrayBuilder::build() {
    glBindTexture(GL_TEXTURE_2D_ARRAY, handle);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    auto texture = Texture(handle, TextureType::Tex2dArray);
    handle = 0;

    return texture;
}

TextureHandle::TextureHandle(const Texture &texture)
        : type(texture.getType()), handle(texture.getRawHandle()) {};

void TextureHandle::bind() const {
    glBindTexture(static_cast<GLenum>(type), handle);
}