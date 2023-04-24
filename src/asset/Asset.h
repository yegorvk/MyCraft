//
// Created by egorv on 4/22/2023.
//

#ifndef SHITCRAFT_ASSET_H
#define SHITCRAFT_ASSET_H

#include <string>
#include <variant>
#include <array>
#include <optional>

#include "renderer/ShaderType.h"

namespace asset {

    struct TextAsset {
        inline explicit TextAsset(std::string &&path) : path(std::move(path)) {}

        std::string path;
    };

    struct ImageAsset {
        inline explicit ImageAsset(std::string &&path) : path(std::move(path)) {}

        std::string path;
    };

    class ShaderAsset {
    public:
        ShaderAsset() = default;

        [[nodiscard]] inline bool contains(ShaderType type) const {
            return stagesRefs[getStageIndex(type)].has_value();
        }

        inline void set(ShaderType type, std::string &&sourceAssetRef) {
            stagesRefs[getStageIndex(type)].emplace(std::move(sourceAssetRef));
        }

        [[nodiscard]] inline const std::string &getSourceAssetRef(ShaderType type) const {
            return stagesRefs[getStageIndex(type)].value();
        }

    private:
        constexpr static std::size_t getStageIndex(ShaderType type) {
            return static_cast<std::size_t>(type);
        }

        std::array<std::optional<std::string>, SHADER_STAGE_COUNT> stagesRefs;
    };

} // asset

#endif //SHITCRAFT_ASSET_H
