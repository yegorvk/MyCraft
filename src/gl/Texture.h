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

template<TexType Type>
using Texture = GlTexture<static_cast<gl_enum_type>(Type)>;

using Texture2d = Texture<TexType::Tex2d>;
using Texture2dMultisample = Texture<TexType::Tex2dMultisample>;
using Texture2dArray = Texture<TexType::Tex2dArray>;
using TextureCubeMap = Texture<TexType::CubeMap>;

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
    struct ArrayTexture2dBuilder {
        ArrayTexture2dBuilder(Tex2dDesc format, int layerCount, TexSamplerOptions options = {});

        void setLayer(int layer, const Image &image);

        [[nodiscard]] Texture2dArray build();

        Tex2dDesc description;
        int layerCount;

        Texture2dArray tex;
    };
}

struct CubeMapTextureBuilder {
    explicit CubeMapTextureBuilder(TexSamplerOptions options = {});

    CubeMapTextureBuilder &setFaceImage(int face, const Image &image);

    [[nodiscard]] TextureCubeMap build();

    TextureCubeMap texture;
};

namespace TextureFactory {
    template<typename It>
    inline Texture2dArray texture2dArray(Tex2dDesc description, It begin, It end,
                                         TexSamplerOptions options = {}) {
        detail::ArrayTexture2dBuilder builder(description, static_cast<int>(std::distance(begin, end)),
                                              options);

        for (int i = 0; begin != end; ++begin, ++i)
            builder.setLayer(i, *begin);

        return builder.build();
    }

    inline CubeMapTextureBuilder textureCubeMap(TexSamplerOptions options = {}) {
        return CubeMapTextureBuilder(options);
    }

    Texture2d texture2d(const Image &image, TexSamplerOptions options = {});

    Texture2d texture2d(int width, int height, int format, int internalFormat, TexSamplerOptions options = {});

    Texture2dMultisample texture2dMultisample(int samples, int width, int height, int internalFormat);
}

#endif //SHITCRAFT_TEXTURE_H
