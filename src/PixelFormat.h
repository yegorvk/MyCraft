//
// Created by egorv on 4/24/2023.
//

#ifndef SHITCRAFT_PIXELFORMAT_H
#define SHITCRAFT_PIXELFORMAT_H

#include "glad/gl.h"

enum class PixelFormat {
    Grayscale = GL_RED,
    Rgb = GL_RGB,
    Rgba = GL_RGBA
};

constexpr GLint getGlPixelFormat(PixelFormat format) {
    return static_cast<GLint>(format);
}

#endif //SHITCRAFT_PIXELFORMAT_H
