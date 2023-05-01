//
// Created by egorv on 4/26/2023.
//

#include <stdexcept>

#include "glad/gl.h"

#include "Vertex.h"
#include "ChunkMesh.h"
#include "ChunkMeshBuilder.h"

ChunkMesh::ChunkMesh(const Texture &arrayTexture) {
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    if (arrayTexture.getType() != TextureType::Tex2dArray)
        throw std::invalid_argument("texture must be array texture");

    arrayTextureId = arrayTexture.getGlHandle();

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

void ChunkMesh::update(const BlockCache &blockCache, const Chunk &chunk, float blockSideLen, glm::vec3 offset) {
    ChunkMeshBuilder builder(chunk, blockCache, offset, blockSideLen);

    vertexCount = builder.getTotalVertexCount();
    auto bufSize = static_cast<int>(sizeof(Vertex) * vertexCount);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bufSize, nullptr, GL_DYNAMIC_DRAW);

    for (int face = 0, bufOffset = 0; face < 6; ++face) {
        const auto &vertices = builder.getVertices(static_cast<BlockFace>(face));
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