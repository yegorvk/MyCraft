//
// Created by egorv on 4/24/2023.
//

#include "spdlog/spdlog.h"
#include "stb_image.h"

#include "Image.h"

Image Image::loadFromMemory(const unsigned char *data, std::size_t size) {
    int width, height, channels;
    auto pixels = stbi_load_from_memory(data, static_cast<int>(size), &width, &height, &channels, 0);

    PixelFormat format;

    switch (channels) {
        case 1:
            format = PixelFormat::Grayscale;
            break;
        case 3:
            format = PixelFormat::Rgb;
            break;
        case 4:
            format = PixelFormat::Rgba;
            break;
        default:
            SPDLOG_ERROR("Unsupported number of image color channels: {}", channels);
            stbi_image_free(pixels);
            return {0, 0, PixelFormat::Grayscale, nullptr};
    }

    return {width, height, format, pixels};
}

Image::Image(Image &&other) noexcept : width(other.width), height(other.height), format(other.format) {
    pixels = other.pixels, other.pixels = nullptr;
}

Image::Image(int width, int height, PixelFormat format, unsigned char *pixels) : width(width),
                                                                                             height(height),
                                                                                             format(format),
                                                                                             pixels(pixels) {}

Image::~Image() {
    if (isValid())
        stbi_image_free(pixels);
}