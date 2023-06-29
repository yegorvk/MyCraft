//
// Created by egorv on 5/17/2023.
//

#ifndef SHITCRAFT_CHUNKMESHDATA_H
#define SHITCRAFT_CHUNKMESHDATA_H

#include <array>
#include <vector>

#include "renderer/ChunkVertex.h"
#include "block/BlockRegistry.h"
#include "Constants.h"

class ChunkMeshDataBuilder;

class ChunkMeshData {
public:
    ChunkMeshData() = default;

    [[nodiscard]] inline int getVertexCount() const {
        return vertexCount;
    }

    [[nodiscard]] inline const std::vector<Vertex> &getVertices(int face) const {
        return vertices[face];
    }

private:
    friend class ChunkMeshDataBuilder;

    int vertexCount = 0;
    std::array<std::vector<Vertex>, 6> vertices;
};

#endif //SHITCRAFT_CHUNKMESHDATA_H
