//
// Created by egorv on 4/26/2023.
//

#include "glad/glad.h"

#include "utils/Utils.h"
#include "chunk/ChunkVertex.h"
#include "ChunkMesh.h"

ChunkMesh::ChunkMesh() {
    VBO = GlVertexBuffer::create();
    VAO = GlVertexArray::create();

    VAO.bind();
    VBO.bind();

    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 2, GL_UNSIGNED_INT, sizeof(PackedChunkVertex), BUF_PTR(PackedChunkVertex, v));
}

void ChunkMesh::update(const ChunkMeshData *meshData) {
    if (meshData == nullptr || meshData->isVoid()) {
        vertexCount = 0;
        return;
    }

    const auto vertices = meshData->getVertices();
    vertexCount = static_cast<int>(vertices.size());
    const auto vertexBufSize = static_cast<int>(sizeof(vertices[0]) * vertexCount);

    VBO.bind();
    glBufferData(GL_ARRAY_BUFFER, vertexBufSize, vertices.data(), GL_DYNAMIC_DRAW);
}

void ChunkMesh::draw() const {
    if (vertexCount != 0) {
        VAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
}