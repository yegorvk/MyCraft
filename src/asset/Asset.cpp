//
// Created by egorv on 5/27/2023.
//

#include <stdexcept>

#include "Asset.h"

namespace asset {
    BlockAssetBuilder BlockAsset::builder() {
        return {};
    }

    BlockAssetBuilder &BlockAssetBuilder::setFace(int face, asset::BlockFace &&value) {
        faces[face] = std::move(value);
        return *this;
    }

    BlockAsset BlockAssetBuilder::build() {
        for (int i = 0; i < 6; ++i) {
            if (!faces[i].has_value())
                throw std::logic_error("cannot create a block asset, not all faces have been set");
        }

        std::array<BlockFace, 6> assetFaces({
                std::move(faces[0].value()),
                std::move(faces[1].value()),
                std::move(faces[2].value()),
                std::move(faces[3].value()),
                std::move(faces[4].value()),
                std::move(faces[5].value())
        });

        return BlockAsset(std::move(assetFaces));
    }
}