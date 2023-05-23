//
// Created by egorv on 4/24/2023.
//

#ifndef SHITCRAFT_PIXELFORMAT_H
#define SHITCRAFT_PIXELFORMAT_H

#include <cstdlib>

#include "glad/glad.h"

enum class PixelFormat {
    Grayscale = GL_RED,
    Rgb = GL_RGB,
    Rgba = GL_RGBA
};

constexpr int getNumChannels(PixelFormat format) {
    switch (format) {
        case PixelFormat::Grayscale:
            return 1;
        case PixelFormat::Rgb:
            return 3;
        case PixelFormat::Rgba:
            return 4;
        default:
            exit(EXIT_FAILURE);
    }
}

constexpr GLint getGlPixelFormat(PixelFormat format) {
    return static_cast<GLint>(format);
}

#endif //SHITCRAFT_PIXELFORMAT_H
