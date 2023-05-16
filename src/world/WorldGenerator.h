//
// Created by egorv on 5/16/2023.
//

#ifndef SHITCRAFT_WORLDGENERATOR_H
#define SHITCRAFT_WORLDGENERATOR_H

#include <random>
#include <cstdlib>

#include "Chunk.h"

class WorldGenerator {
public:
    explicit WorldGenerator(uint64_t seed = 0);

    Chunk generate(glm::ivec3 position);

private:
    [[nodiscard]] static float getNoise(float x, float y) ;

    std::uniform_int_distribution<int> dist;
    std::mt19937 rng;

    float noiseSeed;
};


#endif //SHITCRAFT_WORLDGENERATOR_H
