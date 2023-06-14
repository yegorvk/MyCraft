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

struct TextureDescription {
    constexpr explicit TextureDescription(int width = 0, int height = 0, int channelCount = 0)
            : width(width), height(height), channelCount(channelCount) {}

    int width = 0, height = 0, channelCount = 0;

    [[nodiscard]] constexpr TextureDescription override(const TextureDescription &other) const {
        return TextureDescription(other.width > 0 ? other.width : width,
                                  other.height > 0 ? other.height : height,
                                  other.channelCount > 0 ? other.channelCount : channelCount);
    }
};

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
        Texture2dArrayBuilder(TextureDescription format, int layerCount, TextureOptions options = {});

        void setLayer(int layer, const Image &image);

        [[nodiscard]] Texture build();

        TextureDescription description;
        int layerCount;
        uint handle = 0;
    };
}

class TextureHandle;

class Texture {
public:
    template<typename It>
    static inline Texture texture2dArray(TextureDescription description, It begin, It end,
                                         TextureOptions options = {}) {
        detail::Texture2dArrayBuilder builder(description, static_cast<int>(std::distance(begin, end)),
                                              options);

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

    [[nodiscard]] inline uint getRawHandle() const {
        return handle;
    }

private:
    TextureType type = TextureType::Invalid;
    uint handle = 0;
};

class TextureHandle {
public:
    TextureHandle() = default;

    explicit TextureHandle(const Texture &texture);

    void bind() const;

    [[nodiscard]] inline TextureType getType() const {
        return type;
    }

    [[nodiscard]] inline uint getRawHandle() const {
        return handle;
    }

    [[nodiscard]] inline bool isValid() const {
        return handle != 0;
    }

private:
    TextureType type = TextureType::Invalid;
    uint handle = 0;
};

#endif //SHITCRAFT_TEXTURE_H
