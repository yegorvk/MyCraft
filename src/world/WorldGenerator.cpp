//
// Created by egorv on 5/16/2023.
//

#include <vector>
#include <cmath>

#include <iostream>

#include "glm/glm.hpp"

#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(uint64_t seed) : perlin(seed) {}

ChunkData WorldGenerator::generate(glm::ivec3 position) {
    ChunkData chunk;

    if (position.y < 0)
        return chunk;

    static std::vector<int> heightmap(CHUNK_SIDE_BLOCK_COUNT * CHUNK_SIDE_BLOCK_COUNT);

    for (int x = 0; x < CHUNK_SIDE_BLOCK_COUNT; ++x)
        for (int z = 0; z < CHUNK_SIDE_BLOCK_COUNT; ++z) {
            glm::dvec3 worldPosition = glm::dvec3(position * CHUNK_SIDE_BLOCK_COUNT + glm::ivec3(x, 0, z));

            const double noiseValue = perlin.noise2D(worldPosition.x / 40.0, worldPosition.z / 40.0);

            double height =
                    glm::clamp((noiseValue + 1.0) / 2.0 * 4.5 + 0.4, 0.0, 5.0) *
                    static_cast<double>(CHUNK_SIDE_BLOCK_COUNT);

            heightmap[x * CHUNK_SIDE_BLOCK_COUNT + z] = static_cast<int>(height);
        }

    for (int x = 0; x < CHUNK_SIDE_BLOCK_COUNT; ++x) {
        for (int y = 0; y < CHUNK_SIDE_BLOCK_COUNT; ++y) {
            for (int z = 0; z < CHUNK_SIDE_BLOCK_COUNT; ++z) {
                int absY = position.y * CHUNK_SIDE_BLOCK_COUNT + y;
                glm::ivec3 pos(x, y, z);

                if (absY <= heightmap[x * CHUNK_SIDE_BLOCK_COUNT + z]) {
                    if (absY <= CHUNK_SIDE_BLOCK_COUNT * 5 / 3)
                        chunk.setBlock(pos, 3);
                    else if (absY == heightmap[x * CHUNK_SIDE_BLOCK_COUNT + z] - 1)
                        chunk.setBlock(pos, 1);
                    else if (absY < heightmap[x * CHUNK_SIDE_BLOCK_COUNT + z])
                        chunk.setBlock(pos, 2);
                } else
                    chunk.setBlock(pos, 0);
            }
        }
    }

    return chunk;
}