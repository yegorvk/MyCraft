//
// Created by egorv on 4/10/2023.
//

#ifndef SHITCRAFT_TEXTURE_H
#define SHITCRAFT_TEXTURE_H

#include <cstddef>

#include "glad/gl.h"
#include "types.h"

enum class TexWrapping : GLenum {
    Repeat = GL_REPEAT,
    MirroredRepeat = GL_MIRRORED_REPEAT,
    ClampToEdge = GL_CLAMP_TO_EDGE,
    ClampToBorder = GL_CLAMP_TO_BORDER
};

enum class TexFiltering : GLenum {
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
};

enum class TexType : GLenum {
    Invalid = 0,
    Tex2D = GL_TEXTURE_2D,
};

struct TexOptions {
    TexWrapping wrapping = TexWrapping::Repeat;
    TexFiltering minFilter = TexFiltering::NearestMipmapNearest;
    TexFiltering magFilter = TexFiltering::Nearest;
};

enum PixelFormat : GLenum {
    Grayscale = GL_RED,
    Rgb = GL_RGB,
    Rgba = GL_RGBA
};

class Texture {
public:
    Texture() = default;

    Texture(const Texture &other) = delete;

    inline Texture(Texture &&other) noexcept {
        type = other.type;
        handle = other.handle;
        other.handle = 0;
    }

    static Texture loadTex2dFromBytes(const unsigned char *bytes, std::size_t size, TexOptions options = {});

    static Texture loadTex2dFromMemory(int width,
                                       int height,
                                       PixelFormat format,
                                       const uint8_t *pixels,
                                       TexOptions options = TexOptions());

    inline Texture &operator=(Texture &&other) noexcept = default;

    inline explicit Texture(uint handle, TexType type) : handle(handle), type(type) {}

    inline ~Texture() {
        if (handle != 0)
            glDeleteTextures(1, &handle);
    }

    inline void bind() const {
        glBindTexture(static_cast<GLenum>(type), handle);
    }

private:
    TexType type = TexType::Invalid;
    uint handle = 0;
};

#endif //SHITCRAFT_TEXTURE_H
