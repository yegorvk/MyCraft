//
// Created by egorv on 4/24/2023.
//

#include "spdlog/spdlog.h"
#include "stb_image.h"
#include "stb_image_resize.h"

#include "utils/MathUtils.h"
#include "Image.h"

Image Image::loadFromMemory(const unsigned char *data, std::size_t size, ImageDescription description) {
    stbi_set_flip_vertically_on_load(true);

    ImageDescription inMemoryImageDescription{};

    stbi_info_from_memory(data, static_cast<int>(size), &inMemoryImageDescription.width,
                          &inMemoryImageDescription.height, &inMemoryImageDescription.channelCount);

    description = inMemoryImageDescription.override(description);

    auto pixels = stbi_load_from_memory(data, static_cast<int>(size), &inMemoryImageDescription.width,
                                        &inMemoryImageDescription.height,
                                        nullptr,
                                        description.channelCount);

    if (inMemoryImageDescription.width != description.width || inMemoryImageDescription.height != description.height)
        return Image(ImageDescription(inMemoryImageDescription.width, inMemoryImageDescription.height,
                                      description.channelCount), pixels).resize(description.width, description.height);
    else
        return {description, pixels};
}

Image Image::fromColor(Color color, ImageDescription description) {
    constexpr ImageDescription base(1, 1, 4);

    if (description.channelCount != 4)
        throw std::invalid_argument("Unsupported number of color channel (" + std::to_string(description.channelCount) +
                                    ") for one-color registry (use 4 color channels)");

    description = base.override(description);

    const uint32_t pixelColor = (color.r << 3) + (color.g << 2) + (color.b << 1) + color.a;
    const int area = description.width * description.height;

    auto *pixels = reinterpret_cast<uint32_t *>(malloc(area * sizeof(uint32_t)));

    std::fill(pixels, pixels + area, pixelColor);

    return {description, reinterpret_cast<uint8_t *>(pixels)};
}

Image::Image(Image &&other) noexcept: description(other.description) {
    pixels = other.pixels, other.pixels = nullptr;
}

Image::Image(ImageDescription description, unsigned char *pixels) : description(description), pixels(pixels) {}

Image::~Image() {
    if (pixels)
        free(pixels), pixels = nullptr;
}

Image Image::resize(int newWidth, int newHeight) const {
    auto newPixels = reinterpret_cast<unsigned char *>(malloc(
            sizeof(unsigned char) * newWidth * newHeight * description.channelCount));

    stbir_resize_uint8(
            pixels,
            description.width,
            description.height,
            0,
            newPixels,
            newWidth,
            newHeight,
            0,
            description.channelCount);

    return {ImageDescription(newWidth, newHeight, description.channelCount), newPixels};
}
