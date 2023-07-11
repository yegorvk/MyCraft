//
// Created by egorv on 6/18/2023.
//

#ifndef SHITCRAFT_CHUNKBLOCKS_H
#define SHITCRAFT_CHUNKBLOCKS_H

#include <vector>
#include <array>

#include "Constants.h"

class ChunkBlocks {
public:
    ChunkBlocks();

    [[nodiscard]] inline BlockId getLocalUnchecked(glm::ivec3 position) const {
        return blocks[getIndexFromPosition(position)];
    }

    [[nodiscard]] BlockId getLocalChecked(glm::ivec3 position) const;

    inline void setLocalUnchecked(glm::ivec3 position, BlockId block) {
        if (getLocalUnchecked(position) != block) {
            if (block == 0)
                --setBlockCount;
            else
                ++setBlockCount;
        }

        assert(setBlockCount >= 0);

        blocks[getIndexFromPosition(position)] = block;
    }

    [[nodiscard]] BlockId getAdjacentTo(glm::ivec3 position, int face) const;

    void updateNeighborData(int thisChunkFace, const ChunkBlocks *neighbor);

    [[nodiscard]] constexpr bool isEmpty() const {
        return setBlockCount == 0;
    }

private:
    static constexpr int getIndexFromPosition(glm::ivec3 position) {
        return position.x * CHUNK_SIDE_BLOCK_COUNT * CHUNK_SIDE_BLOCK_COUNT +
               position.y * CHUNK_SIDE_BLOCK_COUNT +
               position.z;
    }

    int setBlockCount = 0;
    std::vector<BlockId> blocks;
    std::array<std::vector<BlockId>, 6> adjFaces;
};


#endif //SHITCRAFT_CHUNKBLOCKS_H
