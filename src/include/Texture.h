//
// Created by egorv on 4/10/2023.
//

#ifndef SHITCRAFT_TEXTURE_H
#define SHITCRAFT_TEXTURE_H

#include <cstddef>
#include "types.h"
#include "glad/gl.h"

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
    Tex2D = GL_TEXTURE_2D,
};

struct TexSamplerOptions {
    TexWrapping wrapping = TexWrapping::Repeat;
    TexFiltering minFilter = TexFiltering::NearestMipmapNearest;
    TexFiltering magFilter = TexFiltering::Nearest;
};

enum PixelFormat : GLenum {
    Grayscale = GL_RED,
    Rgb = GL_RGB,
    Rgba = GL_RGBA
};

template<TexType type>
class Texture {
public:
    Texture() = default;

    Texture(const Texture &other) = delete;

    inline Texture(Texture &&other) noexcept {
        handle = other.handle;
        other.handle = 0;
    }

    inline Texture<type> &operator=(Texture<type> &&other) noexcept = default;

    inline explicit Texture(uint handle) : handle(handle) {}

    inline ~Texture() {
        if (handle != 0)
            glDeleteTextures(1, &handle);
    }

    inline void bind() const {
        glBindTexture(static_cast<GLenum>(type), handle);
    }

private:
    uint handle = 0;
};

class Texture2D : public Texture<TexType::Tex2D> {
public:
    using Texture::Texture;

    static Texture2D loadFromMemory(int width,
                                    int height,
                                    PixelFormat format,
                                    const uint8_t *pixels,
                                    TexSamplerOptions options = TexSamplerOptions());
};

#endif //SHITCRAFT_TEXTURE_H
