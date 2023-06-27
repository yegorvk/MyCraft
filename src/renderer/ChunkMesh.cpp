//
// Created by egorv on 4/26/2023.
//

#include <stdexcept>

#include "glad/glad.h"

#include "Vertex.h"
#include "ChunkMesh.h"

ChunkMesh::ChunkMesh(ChunkMesh &&other) noexcept {
    vao = other.vao, other.vao = 0;
    vbo = other.vbo, other.vbo = 0;
    texture = other.texture;
    vertexCount = other.vertexCount, other.vertexCount = 0;
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

    glEnableVertexAttribArray(0); // aPosition
    glEnableVertexAttribArray(1); // aTexCoords
    glEnableVertexAttribArray(2); // aColor
    glEnableVertexAttribArray(3); // aTextureId
    glEnableVertexAttribArray(4); // aOverlayTextureId

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, position)));

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, texCoords)));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, color)));

    glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex),
                           reinterpret_cast<void *>(offsetof(Vertex, textureId)));

    glBindVertexArray(0);
}

void ChunkMesh::setTilesTexture(TextureHandle tilesTexture) {
    if (tilesTexture.getType() != TextureType::Tex2dArray)
        throw std::invalid_argument("texture must be array texture");

    texture = tilesTexture;
}

void ChunkMesh::update(const ChunkMeshData *meshData) {
    if (meshData == nullptr) {
        vertexCount = 0;
        return;
    }

    vertexCount = meshData->getVertexCount();
    auto bufSize = static_cast<int>(sizeof(Vertex) * vertexCount);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bufSize, nullptr, GL_DYNAMIC_DRAW);

    for (int face = 0, bufOffset = 0; face < 6; ++face) {
        const auto &vertices = meshData->getVertices(face);
        auto size = static_cast<int>(sizeof(Vertex) * vertices.size());

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