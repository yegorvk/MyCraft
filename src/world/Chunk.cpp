//
// Created by egorv on 4/26/2023.
//

#include "spdlog/spdlog.h"

#include "Chunk.h"

Chunk::Chunk(int sideLenLog2) : sideBlockCountLog2(sideLenLog2) {
    blocks.resize(1 << sideLenLog2 << sideLenLog2 << sideLenLog2, 1);

    blocks[0] = 0;
    blocks[74] = 0;
    blocks[(10 << 4 << 4) + (10 << 4) + 10] = 0;
}
