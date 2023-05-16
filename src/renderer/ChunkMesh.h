//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_CHUNKMESH_H
#define SHITCRAFT_CHUNKMESH_H

#include <vector>

#include "types.h"
#include "core/Drawable.h"
#include "world/Chunk.h"
#include "world/BlockCache.h"
#include "Shader.h"
#include "Texture.h"

class ChunkMesh {
public:
    ChunkMesh();

    ChunkMesh(ChunkMesh &&other) noexcept;

    ChunkMesh(const ChunkMesh &other) = delete;

    ~ChunkMesh();

    void setTilesTexture(const Texture &tilesTexture);

    void update(const BlockCache &blockCache, const Chunk &chunk, float blockSideLen, glm::vec3 offset = {});

    void draw() const;

private:
    uint vbo = 0, vao = 0;
    uint arrayTextureId = 0;

    int vertexCount = 0;
};

#endif //SHITCRAFT_CHUNKMESH_H
