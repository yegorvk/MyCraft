//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_CHUNKMESH_H
#define SHITCRAFT_CHUNKMESH_H

#include <vector>
#include <cstdint>

#include "types.h"
#include "chunk/ChunkData.h"
#include "registry/BlockRegistry.h"
#include "gl/Shader.h"
#include "gl/Texture.h"
#include "gl/GlResource.h"

class ChunkMesh {
public:
    ChunkMesh();

    void update(const ChunkMeshData *meshData);

    void draw() const;

private:
    GlVertexArray VAO;
    GlVertexBuffer VBO;

    int vertexCount = 0;
};

#endif //SHITCRAFT_CHUNKMESH_H
