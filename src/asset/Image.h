//
// Created by egorv on 4/24/2023.
//

#ifndef SHITCRAFT_IMAGE_H
#define SHITCRAFT_IMAGE_H

#include <cstddef>

#include "glm/glm.hpp"

#include "types.h"

struct ImageDescription {
    constexpr ImageDescription() = default;

    constexpr ImageDescription(int width, int height, int channelCount) :
            width(width),
            height(height),
            channelCount(channelCount) {}

    [[nodiscard]] constexpr ImageDescription override(const ImageDescription &other) const {
        return {other.width > 0 ? other.width : width,
                other.height > 0 ? other.height : height,
                other.channelCount > 0 ? other.channelCount : channelCount};
    }

    int width, height, channelCount;
};

struct Image {
    static Image loadFromMemory(const unsigned char *data, std::size_t size, ImageDescription description = {});

    static Image fromColor(Color color, ImageDescription description = {});

    Image(const Image &other) = delete;

    Image(Image &&other) noexcept;

    [[nodiscard]] Image resize(int newWidth, int newHeight) const;

    [[nodiscard]] inline int getChannelCount() const {
        return description.channelCount;
    }

    [[nodiscard]] inline bool isValid() const {
        return pixels;
    }

    [[nodiscard]] inline int getWidth() const {
        return description.width;
    }

    [[nodiscard]] inline int getHeight() const {
        return description.height;
    }

    [[nodiscard]] inline const unsigned char *getPixels() const {
        return pixels;
    }

    ~Image();

private:
    Image(ImageDescription description, unsigned char *data);

    const ImageDescription description;
    unsigned char *pixels;
};


#endif //SHITCRAFT_IMAGE_H
