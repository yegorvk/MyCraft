//
// Created by egorv on 4/10/2023.
//

#ifndef SHITCRAFT_TEXTURE_H
#define SHITCRAFT_TEXTURE_H

#include <utility>
#include <cstddef>
#include <algorithm>

#include "types.h"
#include "asset/Image.h"

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

enum class TextureType : GLenum {
    Invalid = 0,
    Tex2d = GL_TEXTURE_2D,
    Tex2dArray = GL_TEXTURE_2D_ARRAY
};

struct TextureOptions {
    TexWrapping wrapping = TexWrapping::Repeat;
    TexFiltering minFilter = TexFiltering::NearestMipmapNearest;
    TexFiltering magFilter = TexFiltering::Nearest;
};

class Texture;

namespace detail {
    struct Texture2dArrayBuilder {
        Texture2dArrayBuilder(int width, int height, int layerCount, PixelFormat format, TextureOptions options = {});

        void setLayer(int layer, const Image &image);

        [[nodiscard]] Texture build();

        int width, height, layerCount;
        PixelFormat format;
        uint handle = 0;
    };
}

class Texture {
public:
    template<typename It>
    static inline Texture texture2dArray(int width, int height, PixelFormat format, It begin, It end,
                                         TextureOptions options = {}) {
        detail::Texture2dArrayBuilder builder(width, height, static_cast<int>(std::distance(begin, end)), format);

        for (int i = 0; begin != end; ++begin, ++i)
            builder.setLayer(i, *begin);

        return builder.build();
    }

    static Texture texture2d(const Image &image, TextureOptions options = {});

    Texture() = default;

    Texture(uint handle, TextureType type);

    Texture(const Texture &other) = delete;

    Texture(Texture &&other) noexcept;

    inline Texture &operator=(const Texture &other) = delete;

    Texture &operator=(Texture &&other) noexcept;

    ~Texture();

    void bind() const;

    [[nodiscard]] inline TextureType getType() const {
        return type;
    }

    [[nodiscard]] inline uint getGlHandle() const {
        return handle;
    }

private:
    TextureType type = TextureType::Invalid;
    uint handle = 0;
};

#endif //SHITCRAFT_TEXTURE_H
