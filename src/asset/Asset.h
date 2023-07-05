//
// Created by egorv on 4/22/2023.
//

#ifndef SHITCRAFT_ASSET_H
#define SHITCRAFT_ASSET_H

#include <string>
#include <variant>
#include <array>
#include <optional>

#include "glm/glm.hpp"

#include "gl/ShaderType.h"
#include "types.h"
#include "Image.h"

namespace asset {

    struct TextAsset {
        inline explicit TextAsset(std::string &&path) : path(std::move(path)) {}

        std::string path;
    };

    struct ImageAsset {
        inline ImageAsset(bool containsColor, std::string colorOrPath, ImageDescription description = {})
            : onePixelImage(containsColor), colorOrPath(std::move(colorOrPath)), description(description) {}

        bool onePixelImage;
        std::string colorOrPath;
        ImageDescription description;
    };

    struct ColorAsset {
        inline explicit ColorAsset(glm::u8vec4 color) : color(color) {}

        glm::u8vec4 color;
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

    struct BlockFace {
    public:
        inline explicit BlockFace(std::string textureRef) : textureRef(std::move(textureRef)) {};

        [[nodiscard]] inline const std::string &getTextureRef() const {
            return textureRef;
        }

    private:
        std::string textureRef;
    };

    class BlockAssetBuilder;

    class BlockAsset {
    public:
        static BlockAssetBuilder builder();

        [[nodiscard]] inline const BlockFace &getFace(int face) const {
            return faces[face];
        }

    private:
        friend class BlockAssetBuilder;

        explicit inline BlockAsset(std::array<BlockFace, 6> &&faces)
                : faces(std::move(faces)) {};

        bool translucent = false;
        std::array<BlockFace, 6> faces;
    };

    class BlockAssetBuilder {
    public:
        BlockAssetBuilder &setFace(int face, BlockFace &&value);

        BlockAsset build();

    private:
        friend class BlockAsset;

        BlockAssetBuilder() = default;

        std::array<std::optional<BlockFace>, 6> faces;
    };

} // asset

#endif //SHITCRAFT_ASSET_H
