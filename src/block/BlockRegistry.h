//
// Created by egorv on 4/27/2023.
//

#ifndef SHITCRAFT_BLOCKREGISTRY_H
#define SHITCRAFT_BLOCKREGISTRY_H

#include <vector>
#include <span>

#include "asset/Asset.h"
#include "renderer/TextureManager.h"
#include "Block.h"
#include "arch.h"

constexpr std::size_t MAX_BLOCK_ID = 1000;

class BlockCacheBuilder;

class BlockRegistry {
public:
    BlockRegistry();

    inline void registerBlock(uint16_t id, const std::string& ref, const asset::Provider &provider) {
        registerBlock(id, provider.getBlock(ref), provider);
    }

    void registerBlock(uint16_t id, const asset::BlockAsset &blockAsset, const asset::Provider &provider);

    [[nodiscard]] inline ALWAYS_INLINE const Block &getBlock(uint16_t id) const {
        return blocks[id - 1];
    }

    [[nodiscard]] inline const TextureManager &getTextureManager() const {
        return texManager;
    }

private:
    TextureManager texManager;
    std::vector<Block> blocks;
};

#endif //SHITCRAFT_BLOCKREGISTRY_H
