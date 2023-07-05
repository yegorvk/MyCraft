//
// Created by egorv on 4/27/2023.
//

#ifndef SHITCRAFT_BLOCKREGISTRY_H
#define SHITCRAFT_BLOCKREGISTRY_H

#include <vector>
#include <span>

#include "asset/Asset.h"
#include "TextureManager.h"
#include "block/Block.h"
#include "arch.h"

using block_id_type = uint16_t;

constexpr block_id_type MAX_BLOCK_ID = 1000;

class BlockRegistry {
public:
    BlockRegistry();

    inline void registerBlock(block_id_type id, const std::string& ref, const asset::Provider &provider) {
        registerBlock(id, provider.getBlock(ref), provider);
    }

    void registerBlock(block_id_type id, const asset::BlockAsset &blockAsset, const asset::Provider &provider);

    [[nodiscard]] inline const Block &getBlock(block_id_type id) const {
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
