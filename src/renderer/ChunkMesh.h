//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_CHUNKMESH_H
#define SHITCRAFT_CHUNKMESH_H

#include <vector>
#include <cstdint>

#include "types.h"
#include "chunk/ChunkData.h"
#include "block/BlockRegistry.h"
#include "Shader.h"
#include "Texture.h"

class ChunkMesh {
public:
    ChunkMesh();

    ChunkMesh(ChunkMesh &&other) noexcept;

    ChunkMesh(const ChunkMesh &other) = delete;

    ~ChunkMesh();

    void setTilesTexture(TextureHandle tilesTexture);

    void update(const ChunkMeshData &meshData);

    void draw() const;

private:
    uint vbo = 0, vao = 0;
    TextureHandle texture;

    int vertexCount = 0;
};

#endif //SHITCRAFT_CHUNKMESH_H
