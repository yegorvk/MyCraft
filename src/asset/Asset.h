//
// Created by egorv on 4/16/2023.
//

#ifndef SHITCRAFT_ASSET_H
#define SHITCRAFT_ASSET_H

#include <string>
#include <variant>

struct TextAsset {
    std::string path;
};

struct TextureAsset {
    std::string path;
};

struct ShaderAsset {
    TextAsset vertex, fragment;
};

struct BlockFace {
    TextureAsset texture;
    bool translucent = false;
};

struct BlockAsset {
    BlockFace front, back, left, right, top, bottom;
};

// No group type, because it is handled during parsing
enum class AssetType {
    Text = 0,
    Texture = 1,
    Shader = 2,
    Block = 3
};

class Asset {
public:
    explicit Asset(TextAsset &&asset) : asset(asset) {}

    explicit Asset(TextureAsset &&asset) : asset(asset) {}

    explicit Asset(ShaderAsset &&asset) : asset(asset) {}

    explicit Asset(BlockAsset &&asset) : asset(asset) {}

    inline AssetType getType() {
        return static_cast<AssetType>(asset.index());
    }

    inline const TextAsset &getText() {
        return std::get<TextAsset>(asset);
    }

    inline const TextureAsset &getTexture() {
        return std::get<TextureAsset>(asset);
    }

    inline const ShaderAsset &getShader() {
        return std::get<ShaderAsset>(asset);
    }

    inline const BlockAsset &getBlock() {
        return std::get<BlockAsset>(asset);
    }
private:
    std::variant<TextAsset, TextureAsset, ShaderAsset, BlockAsset> asset;
};


#endif //SHITCRAFT_ASSET_H
