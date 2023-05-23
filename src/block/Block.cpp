//
// Created by egorv on 4/27/2023.
//

#include <algorithm>

#include "Block.h"

Block::Block(uint rightTex, uint leftTex, uint topTex, uint bottomTex, uint frontTex, uint backTex) {
    faceTextureIndices[0] = rightTex;
    faceTextureIndices[1] = leftTex;
    faceTextureIndices[2] = topTex;
    faceTextureIndices[3] = bottomTex;
    faceTextureIndices[4] = frontTex;
    faceTextureIndices[5] = backTex;
}