//
// Created by egorv on 5/16/2023.
//

#include <vector>
#include <cmath>

#include <iostream>

#include "SimplexNoise.h"

#include "WorldGenerator.h"

inline float computeFloatSeed(uint64_t seed) {
    const auto seed32 = static_cast<uint32_t>(seed);
    constexpr uint32_t mask = (1u << 24u) - 1u;
    const auto rawExp = static_cast<int32_t>((seed32 & (~mask)) >> 24u);
    return ldexpf(static_cast<float>(seed32 & mask), glm::clamp(rawExp, -32, 32));
}

WorldGenerator::WorldGenerator(uint64_t seed) : rng(seed), noiseSeed(computeFloatSeed(seed)) {
    SimplexNoise::seed(seed);
}

Chunk WorldGenerator::generate(glm::ivec3 position) {
    Chunk chunk;

    std::vector<int> heightmap(1 << CHUNK_SIDE_BLOCK_COUNT_LOG2 << CHUNK_SIDE_BLOCK_COUNT_LOG2);

    for (int x = 0; x < (1 << CHUNK_SIDE_BLOCK_COUNT_LOG2); ++x)
        for (int z = 0; z < (1 << CHUNK_SIDE_BLOCK_COUNT_LOG2); ++z) {
            float noiseValue = getNoise(static_cast<float>((position.x << CHUNK_SIDE_BLOCK_COUNT_LOG2) + x) / 20.f,
                                        static_cast<float>((position.z << CHUNK_SIDE_BLOCK_COUNT_LOG2) + z) / 20.f);

            float height = glm::clamp((noiseValue + 1.f) * 0.3f + 0.4f, 0.f, 1.f) *
                           static_cast<float>(1 << CHUNK_SIDE_BLOCK_COUNT_LOG2);

            heightmap[(x << CHUNK_SIDE_BLOCK_COUNT_LOG2) + z] = static_cast<int>(height);
        }

    for (int x = 0; x < (1 << CHUNK_SIDE_BLOCK_COUNT_LOG2); ++x) {
        for (int y = 0; y < (1 << CHUNK_SIDE_BLOCK_COUNT_LOG2); ++y) {
            for (int z = 0; z < (1 << CHUNK_SIDE_BLOCK_COUNT_LOG2); ++z) {
                if (y < heightmap[(x << CHUNK_SIDE_BLOCK_COUNT_LOG2) + z])
                    chunk.setBlock(x, y, z, 1);
                else
                    chunk.setBlock(x, y, z, 0);
            }
        }
    }

    return chunk;
}

float WorldGenerator::getNoise(float x, float y) {
    return SimplexNoise::noise(x, y);
}