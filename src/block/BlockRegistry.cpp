//
// Created by egorv on 4/27/2023.
//

#include <stdexcept>

#include "BlockRegistry.h"

BlockRegistry::BlockRegistry() {
    blocks.resize(MAX_BLOCK_ID + 1);
}

void BlockRegistry::registerBlock(uint16_t id, const asset::BlockAsset &blockAsset, const asset::Provider &provider) {
    texManager.registerBlock(blockAsset, provider);

    if (id > MAX_BLOCK_ID)
        throw std::invalid_argument("blockAsset id must be less than " + std::to_string(MAX_BLOCK_ID));
    else if (id == 0)
        throw std::invalid_argument("blockAsset id must be greater than 0");

    Block block;

    for (int i = 0; i < 6; ++i) {
        const auto &faceAsset = blockAsset.getFace(i);

        const auto textureId = texManager.getId(faceAsset.getTextureRef());

        block.setFace(i, BlockFace(textureId));
    }

    blocks[id - 1] = block;
}