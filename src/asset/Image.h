//
// Created by egorv on 4/24/2023.
//

#ifndef SHITCRAFT_IMAGE_H
#define SHITCRAFT_IMAGE_H

#include <cstddef>

#include "PixelFormat.h"

struct Image {
    static Image loadFromMemory(const unsigned char *data, std::size_t size);

    Image(const Image &other) = delete;

    Image(Image &&other) noexcept;

    Image resize(int newWidth, int newHeight) const;

    [[nodiscard]] inline PixelFormat getFormat() const {
        return format;
    }

    [[nodiscard]] inline bool isValid() const {
        return pixels;
    }

    [[nodiscard]] inline int getWidth() const {
        return width;
    }

    [[nodiscard]] inline int getHeight() const {
        return height;
    }

    [[nodiscard]] inline const unsigned char *getPixels() const {
        return pixels;
    }

    ~Image();

private:
    Image(int width, int height, PixelFormat format, unsigned char *data);

    const int width, height;
    const PixelFormat format;
    unsigned char *pixels;
};


#endif //SHITCRAFT_IMAGE_H
