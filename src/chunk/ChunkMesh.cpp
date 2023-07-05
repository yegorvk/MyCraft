//
// Created by egorv on 4/26/2023.
//

#include <stdexcept>

#include "glad/glad.h"

#include "chunk/ChunkVertex.h"
#include "ChunkMesh.h"

ChunkMesh::ChunkMesh(ChunkMesh &&other) noexcept {
    vao = other.vao;
    other.vao = 0;
    vbo = other.vbo;
    other.vbo = 0;
    texture = other.texture;
    vertexCount = other.vertexCount;
    other.vertexCount = 0;
}

ChunkMesh::~ChunkMesh() {
    if (vao != 0)
        glDeleteVertexArrays(1, &vao);

    if (vbo != 0)
        glDeleteBuffers(1, &vbo);
}

ChunkMesh::ChunkMesh() {
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);

    glVertexAttribIPointer(0, 2, GL_UNSIGNED_INT, sizeof(PackedChunkVertex),
                          reinterpret_cast<void *>(offsetof(PackedChunkVertex, v)));

    glBindVertexArray(0);
}

void ChunkMesh::setTilesTexture(TextureHandle tilesTexture) {
    if (tilesTexture.getType() != TextureType::Tex2dArray)
        throw std::invalid_argument("registry must be array registry");

    texture = tilesTexture;
}

void ChunkMesh::update(const ChunkMeshData *meshData) {
    if (meshData == nullptr) {
        vertexCount = 0;
        return;
    }

    vertexCount = meshData->getVertexCount();
    auto bufSize = static_cast<int>(sizeof(PackedChunkVertex) * vertexCount);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bufSize, nullptr, GL_DYNAMIC_DRAW);

    for (int face = 0, bufOffset = 0; face < 6; ++face) {
        const auto &vertices = meshData->getVertices(face);
        auto size = static_cast<int>(sizeof(PackedChunkVertex) * vertices.size());

        glBufferSubData(GL_ARRAY_BUFFER, bufOffset, size, vertices.data());

        bufOffset += size;
    }
}

void ChunkMesh::draw() const {
    if (texture.isValid() && vertexCount != 0) {
        texture.bind();
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        glBindVertexArray(0);
    }
}