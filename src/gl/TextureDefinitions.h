//
// Created by egorv on 7/12/2023.
//

#ifndef SHITCRAFT_TEXTUREDEFINITIONS_H
#define SHITCRAFT_TEXTUREDEFINITIONS_H

#include "chunk/Constants.h"

#include "glad/glad.h"

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
    Tex2dMultisample = GL_TEXTURE_2D_MULTISAMPLE,
    CubeMap = GL_TEXTURE_CUBE_MAP
};

constexpr int getTextureFormat(int channelCount) {
    switch (channelCount) {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            return 0;
    }
}

constexpr int getTextureInternalFormat(int channelCount) {
    switch (channelCount) {
        case 1:
            return GL_R8;
        case 2:
            return GL_RG8;
        case 3:
            return GL_RGB8;
        case 4:
            return GL_RGBA8;
        default:
            return 0;
    }
}

constexpr int getCubeMapFace(int face) {
    return GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
}

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
        return {TexWrapMode::ClampToEdge, TexFilter::Linear, TexFilter::Linear};
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

#endif //SHITCRAFT_TEXTUREDEFINITIONS_H
