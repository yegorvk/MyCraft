//
// Created by egorv on 4/27/2023.
//

#include <algorithm>

#include "Block.h"

Block::Block() {
    std::fill(faceTextureIndices.begin(), faceTextureIndices.end(), 1);
}