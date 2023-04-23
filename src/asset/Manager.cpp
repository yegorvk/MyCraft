//
// Created by egorv on 4/22/2023.
//

#include "renderer/Texture.h"
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

    const Texture &Manager::getTexture(const std::string &key) const {
        return cache.get<Texture, Resolver<Texture>>(key, Resolver<Texture>(*this));
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
    Texture Manager::resolve<Texture>(const std::string &ref) const {
        const auto &asset = index.get<TextureAsset>(ref);
        auto bytes = reader->getBytes(asset.path.c_str());

        return Texture::loadTex2dFromBytes(bytes.data(), bytes.size());
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