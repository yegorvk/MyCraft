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

    if (position.y != 0)
        return chunk;

    std::vector<int> heightmap(CHUNK_SIDE_BLOCK_COUNT * CHUNK_SIDE_BLOCK_COUNT);

    for (int x = 0; x < CHUNK_SIDE_BLOCK_COUNT; ++x)
        for (int z = 0; z < CHUNK_SIDE_BLOCK_COUNT; ++z) {
            glm::dvec3 worldPosition =
                    BLOCK_SIDE_SCALE * glm::dvec3(position * CHUNK_SIDE_BLOCK_COUNT + glm::ivec3(x, 0, z));

            const double noiseValue = perlin.noise2D(worldPosition.x / 5.0, worldPosition.z / 5.0);

            double height =
                    glm::clamp((noiseValue + 1.0) * 0.3 + 0.4, 0.0, 1.0) * static_cast<double>(CHUNK_SIDE_BLOCK_COUNT);

            heightmap[x * CHUNK_SIDE_BLOCK_COUNT + z] = static_cast<int>(height);
        }

    for (int x = 0; x < CHUNK_SIDE_BLOCK_COUNT; ++x) {
        for (int y = 0; y < CHUNK_SIDE_BLOCK_COUNT; ++y) {
            for (int z = 0; z < CHUNK_SIDE_BLOCK_COUNT; ++z) {
                if (y < heightmap[x * CHUNK_SIDE_BLOCK_COUNT + z])
                    chunk.setBlock(glm::ivec3(x, y, z), 1);
                else
                    chunk.setBlock(glm::ivec3(x, y, z), 0);
            }
        }
    }

    return chunk;
}