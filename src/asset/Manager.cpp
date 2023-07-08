//
// Created by egorv on 4/22/2023.
//

#include <sstream>

#include "gl/Shader.h"

#include "Manager.h"
#include "Parser.h"

namespace asset {
    Manager Manager::createDefault(std::unique_ptr<FileReader> reader) {
        Index index;

        Parser parser(index, *reader, "asset.schema.json");
        parser.parse();

        return {std::move(index), std::move(reader)};
    }

    asset::Manager::Manager(Index &&index, std::unique_ptr<FileReader> reader) : index(std::forward<Index>(index)),
                                                                                 reader(std::move(reader)) {}

    const std::string &Manager::getText(const std::string &key) const {
        return cache.get<std::string, Resolver<std::string>>(key, Resolver<std::string>(*this));
    }

    const Image &Manager::getImage(const std::string &key) const {
        return cache.get<Image, Resolver<Image>>(key, Resolver<Image>(*this));
    }

    const Shader &Manager::getShader(const std::string &key) const {
        return cache.get<Shader, Resolver<Shader>>(key, Resolver<Shader>(*this));
    }

    const BlockAsset &Manager::getBlock(const std::string &key) const {
        return cache.get<BlockAsset, Resolver<BlockAsset>>(key, Resolver<BlockAsset>(*this));
    }

    template<>
    std::string Manager::resolve<std::string>(const std::string &ref) const {
        const auto &asset = index.get<TextAsset>(ref);
        return reader->getText(asset.path);
    }

    template<>
    Image Manager::resolve<Image>(const std::string &ref) const {
        const auto &asset = index.get<ImageAsset>(ref);

        if (!asset.onePixelImage) {
            auto bytes = reader->getBytes(asset.colorOrPath);

            return Image::loadFromMemory(bytes.data(), bytes.size(), asset.description);
        } else {
            return Image::fromColor(parseColor(asset.colorOrPath), asset.description);
        }
    }

    template<>
    Shader Manager::resolve<Shader>(const std::string &ref) const {
        const auto &asset = index.get<ShaderAsset>(ref);
        ShaderBuilder builder;

        for (int i = 0; i < SHADER_STAGE_COUNT; ++i) {
            auto type = static_cast<ShaderType>(i);

            if (asset.contains(type)) {
                if(!builder.setStage(type, getText(asset.getSourceAssetRef(type)).c_str()))
                    return Shader(0);
            }
        }

        return builder.link();
    }

    template<>
    BlockAsset Manager::resolve<BlockAsset>(const std::string &ref) const {
        return index.get<BlockAsset>(ref);
    }

    Color Manager::parseColor(std::string_view color) {
        std::stringstream ss(color.substr(1).data());

        int colorInt;
        ss >> std::hex >> colorInt;

        if (color.size() <= 1 + 2)
            return {colorInt, colorInt, colorInt, colorInt};
        else if (color.size() <= 1 + 2 * 3)
            return {(colorInt >> 16) & 0xFF, (colorInt >> 8) & 0xFF, colorInt & 0xFF, 0xFF};
        else
            return {(colorInt >> 24) & 0xFF, (colorInt >> 16) & 0xFF, (colorInt >> 8) & 0xFF, colorInt & 0xFF};
    }
}