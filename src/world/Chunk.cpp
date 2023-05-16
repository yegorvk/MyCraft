//
// Created by egorv on 4/26/2023.
//

#include "spdlog/spdlog.h"

#include "Chunk.h"

Chunk::Chunk(int sideLenLog2) : sideBlockCountLog2(sideLenLog2) {
    blocks.resize(1 << sideLenLog2 << sideLenLog2 << sideLenLog2, 0);
}
