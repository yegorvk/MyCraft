//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_CHUNKMESHDATA_H
#define SHITCRAFT_CHUNKMESHDATA_H

#include <array>
#include <vector>

#include "ChunkVertex.h"
#include "registry/BlockRegistry.h"
#include "Constants.h"

class ChunkMeshDataBuilder;

class ChunkMeshData {
public:
    ChunkMeshData() = default;

    [[nodiscard]] inline const std::vector<PackedChunkVertex> &getVertices() const {
        return vertices;
    }

    [[nodiscard]] inline bool isVoid() const {
        return vertices.empty();
    }

private:
    friend class ChunkMeshDataBuilder;

    std::vector<PackedChunkVertex> vertices;
};

#endif //SHITCRAFT_CHUNKMESHDATA_H
