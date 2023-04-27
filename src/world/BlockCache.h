//
// Created by egorv on 4/27/2023.
//

#ifndef SHITCRAFT_BLOCKCACHE_H
#define SHITCRAFT_BLOCKCACHE_H

#include <vector>

#include "Block.h"

class BlockCache {
public:
    BlockCache();

    [[nodiscard]] inline const Block &getBlock(uint16_t id) const {
        return blocks[id - 1];
    }
private:
    std::vector<Block> blocks;
};


#endif //SHITCRAFT_BLOCKCACHE_H
