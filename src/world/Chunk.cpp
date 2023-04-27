//
// Created by egorv on 4/26/2023.
//

#include "spdlog/spdlog.h"

#include "Chunk.h"

Chunk::Chunk(int size) : sideLen(size) {
    blocks.resize(size*size*size, 1);
}
