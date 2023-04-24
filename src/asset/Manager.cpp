//
// Created by egorv on 4/22/2023.
//

#include "renderer/Shader.h"

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

    template<>
    std::string Manager::resolve<std::string>(const std::string &ref) const {
        const auto &asset = index.get<TextAsset>(ref);
        return reader->getText(asset.path.c_str());
    }

    template<>
    Image Manager::resolve<Image>(const std::string &ref) const {
        const auto &asset = index.get<ImageAsset>(ref);
        auto bytes = reader->getBytes(asset.path.c_str());

        return Image::loadFromMemory(bytes.data(), bytes.size());
    }

    template<>
    Shader Manager::resolve<Shader>(const std::string &ref) const {
        const auto &asset = index.get<ShaderAsset>(ref);
        ShaderBuilder builder;

        for (int i = 0; i < SHADER_STAGE_COUNT; ++i) {
            auto type = static_cast<ShaderType>(i);

            if (asset.contains(type))
                builder.setStage(type, getText(asset.getSourceAssetRef(type)).c_str());
        }

        return builder.link();
    }
}