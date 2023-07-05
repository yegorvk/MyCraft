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

    [[nodiscard]] inline int getVertexCount() const {
        return vertexCount;
    }

    [[nodiscard]] inline const std::vector<PackedChunkVertex> &getVertices(int face) const {
        return vertices[face];
    }

private:
    friend class ChunkMeshDataBuilder;

    int vertexCount = 0;
    std::array<std::vector<PackedChunkVertex>, 6> vertices;
};

#endif //SHITCRAFT_CHUNKMESHDATA_H
