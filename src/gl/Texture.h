//
// Created by egorv on 4/10/2023.
//

#ifndef SHITCRAFT_TEXTURE_H
#define SHITCRAFT_TEXTURE_H

#include <utility>
#include <cstddef>
#include <algorithm>

#include "glad/glad.h"

#include "asset/Image.h"
#include "GlTypes.h"
#include "GlResource.h"

enum class TexWrapMode : gl_enum_type {
    Repeat = GL_REPEAT,
    MirroredRepeat = GL_MIRRORED_REPEAT,
    ClampToEdge = GL_CLAMP_TO_EDGE,
    ClampToBorder = GL_CLAMP_TO_BORDER
};

enum class TexFilter : gl_enum_type {
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
};

enum class TexType : gl_enum_type {
    Invalid = 0,
    Tex2d = GL_TEXTURE_2D,
    Tex2dArray = GL_TEXTURE_2D_ARRAY,
    Tex2dMultisample = GL_TEXTURE_2D_MULTISAMPLE
};

constexpr float NO_ANISOTROPY = 0.f;

struct TexSamplerOptions {
    constexpr TexSamplerOptions() = default;

    constexpr TexSamplerOptions(TexWrapMode wrapMode, TexFilter minFilter, TexFilter magFilter,
                                float maxAnisotropy = NO_ANISOTROPY, float loadBias = 0.f)
            : wrapMode(wrapMode), minFilter(minFilter), magFilter(magFilter), lodBias(loadBias), maxAnisotropy(maxAnisotropy) {}

    static constexpr TexSamplerOptions nearestClamp() {
        return {TexWrapMode::ClampToEdge, TexFilter::Nearest, TexFilter::Nearest};
    }

    static constexpr TexSamplerOptions linearClamp() {
        return {TexWrapMode::ClampToEdge, TexFilter::Linear, TexFilter::Nearest};
    }

    static constexpr TexSamplerOptions nearestRepeat() {
        return {TexWrapMode::Repeat, TexFilter::Nearest, TexFilter::Nearest};
    }

    static constexpr TexSamplerOptions nearestMipmapNearestRepeat() {
        return {TexWrapMode::Repeat, TexFilter::NearestMipmapNearest, TexFilter::Nearest};
    }

    static constexpr TexSamplerOptions linearMipmapLinearRepeat() {
        return {TexWrapMode::Repeat, TexFilter::LinearMipmapLinear, TexFilter::Linear};
    }

    static constexpr TexSamplerOptions linearMipmapNearestRepeat() {
        return {TexWrapMode::Repeat, TexFilter::LinearMipmapNearest, TexFilter::Linear};
    }

    [[nodiscard]] constexpr bool usesMipmaps() const {
        return minFilter != TexFilter::Linear && minFilter != TexFilter::Nearest;
    }

    TexWrapMode wrapMode = TexWrapMode::ClampToEdge;
    TexFilter minFilter = TexFilter::Nearest;
    TexFilter magFilter = TexFilter::Nearest;
    float lodBias = 0.f;
    float maxAnisotropy = NO_ANISOTROPY;
};

template<TexType Type>
using Texture = GlTexture<static_cast<gl_enum_type>(Type)>;

template<TexType Type>
using TextureRef = Texture<Type>::ref_type;

using Texture2d = Texture<TexType::Tex2d>;
using Texture2dMultisample = Texture<TexType::Tex2dMultisample>;
using Texture2dArray = Texture<TexType::Tex2dArray>;

struct Tex2dDesc {
    constexpr explicit Tex2dDesc(int width = 0, int height = 0, int channelCount = 0)
            : width(width), height(height), channelCount(channelCount) {}

    int width = 0, height = 0, channelCount = 0;

    [[nodiscard]] constexpr Tex2dDesc override(const Tex2dDesc &other) const {
        return Tex2dDesc(other.width > 0 ? other.width : width,
                         other.height > 0 ? other.height : height,
                         other.channelCount > 0 ? other.channelCount : channelCount);
    }
};

namespace detail {
    struct Texture2dArrayBuilder {
        Texture2dArrayBuilder(Tex2dDesc format, int layerCount, TexSamplerOptions options = {});

        void setLayer(int layer, const Image &image);

        [[nodiscard]] Texture2dArray build();

        Tex2dDesc description;
        int layerCount;

        Texture2dArray tex;
    };
}

namespace TextureFactory {
    template<typename It>
    inline Texture2dArray texture2dArray(Tex2dDesc description, It begin, It end,
                                         TexSamplerOptions options = {}) {
        detail::Texture2dArrayBuilder builder(description, static_cast<int>(std::distance(begin, end)),
                                              options);

        for (int i = 0; begin != end; ++begin, ++i)
            builder.setLayer(i, *begin);

        return builder.build();
    }

    Texture2d texture2d(const Image &image, TexSamplerOptions options = {});

    Texture2d texture2d(int width, int height, int format, int internalFormat, TexSamplerOptions options = {});

    Texture2dMultisample texture2dMultisample(int samples, int width, int height, int internalFormat);
}

#endif //SHITCRAFT_TEXTURE_H
