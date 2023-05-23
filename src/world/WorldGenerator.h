//
// Created by egorv on 5/16/2023.
//

#ifndef SHITCRAFT_WORLDGENERATOR_H
#define SHITCRAFT_WORLDGENERATOR_H

#include <random>
#include <cstdlib>

#include "PerlinNoise.hpp"

#include "chunk/ChunkData.h"

class WorldGenerator {
public:
    explicit WorldGenerator(uint64_t seed = 0);

    ChunkData generate(glm::ivec3 position);

private:
    siv::PerlinNoise perlin;
};

#endif //SHITCRAFT_WORLDGENERATOR_H
