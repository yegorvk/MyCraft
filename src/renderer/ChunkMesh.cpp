//
// Created by egorv on 4/26/2023.
//

#include <stdexcept>

#include "glad/gl.h"

#include "Vertex.h"
#include "ChunkMesh.h"
#include "ChunkMeshBuilder.h"

ChunkMesh::ChunkMesh(ChunkMesh &&other)  noexcept {
    vao = other.vao, other.vao = 0;
    vbo = other.vbo, other.vbo = 0;
    arrayTextureId = other.arrayTextureId;
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, position)));

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, texCoords)));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, color)));

    glBindVertexArray(0);
}

void ChunkMesh::setTilesTexture(const Texture &tilesTexture) {
    if (tilesTexture.getType() != TextureType::Tex2dArray)
        throw std::invalid_argument("texture must be array texture");

    arrayTextureId = tilesTexture.getGlHandle();
}

void ChunkMesh::update(const BlockCache &blockCache, const Chunk &chunk, float blockSideLen, glm::vec3 offset) {
    ChunkMeshBuilder builder(chunk, blockCache, offset, blockSideLen);

    vertexCount = builder.getTotalVertexCount();
    auto bufSize = static_cast<int>(sizeof(Vertex) * vertexCount);

    if (vbo == 0) {
        int k;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bufSize, nullptr, GL_DYNAMIC_DRAW);

    for (int face = 0, bufOffset = 0; face < 6; ++face) {
        const auto &vertices = builder.getVertices(face);
        auto size = static_cast<int>(sizeof(Vertex) * vertices.size());

        glBufferSubData(GL_ARRAY_BUFFER, bufOffset, size, vertices.data());

        bufOffset += size;
    }
}

void ChunkMesh::draw() const {
    glBindTexture(GL_TEXTURE_2D_ARRAY, arrayTextureId);
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    glBindVertexArray(0);
}