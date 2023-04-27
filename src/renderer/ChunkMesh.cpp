//
// Created by egorv on 4/26/2023.
//

#include <stdexcept>

#include "glad/gl.h"

#include "Vertex.h"
#include "ChunkMesh.h"

#define VERTEX(x, y, z, s, t) Vertex(glm::vec3(x, y, z), glm::vec3(s, t, 0.f))

static Vertex CUBE_VERTICES[] = {
        // Front face

        VERTEX(-0.5f, -0.5f, 0.5f, 0.f, 0.f), // bottom left
        VERTEX(0.5f, -0.5f, 0.5f, 1.f, 0.f), // bottom right
        VERTEX(0.5f, 0.5f, 0.5f, 1.f, 1.f), // top right
        VERTEX(-0.5f, -0.5f, 0.5f, 0.f, 0.f), // bottom left
        VERTEX(0.5f, 0.5f, 0.5f, 1.f, 1.f), // top right
        VERTEX(-0.5f, 0.5f, 0.5f, 0.f, 1.f), // top left

        // Back face

        VERTEX(-0.5f, -0.5f, -0.5f, 1.f, 0.f), // bottom left
        VERTEX(-0.5f, 0.5f, -0.5f, 1.f, 1.f), // top left
        VERTEX(0.5f, 0.5f, -0.5f, 0.f, 1.f), // top right
        VERTEX(-0.5f, -0.5f, -0.5f, 1.f, 0.f), // bottom left
        VERTEX(0.5f, 0.5f, -0.5f, 0.f, 1.f), // top right
        VERTEX(0.5f, -0.5f, -0.5f, 0.f, 0.f), // bottom right

        // Left face

        VERTEX(-0.5f, -0.5f, 0.5f, 1.f, 0.f),
        VERTEX(-0.5f, 0.5f, 0.5f, 1.f, 1.f),
        VERTEX(-0.5f, 0.5f, -0.5f, 0.f, 1.f),
        VERTEX(-0.5f, -0.5f, 0.5f, 1.f, 0.f),
        VERTEX(-0.5f, 0.5f, -0.5f, 0.f, 1.f),
        VERTEX(-0.5f, -0.5f, -0.5f, 0.f, 0.f),

        // Right face

        VERTEX(0.5f, -0.5f, 0.5f, 0.f, 0.f),
        VERTEX(0.5f, -0.5f, -0.5f, 1.f, 0.f),
        VERTEX(0.5f, 0.5f, -0.5f, 1.f, 1.f),
        VERTEX(0.5f, -0.5f, 0.5f, 0.f, 0.f),
        VERTEX(0.5f, 0.5f, -0.5f, 1.f, 1.f),
        VERTEX(0.5f, 0.5f, 0.5f, 0.f, 1.f),

        // Top face

        VERTEX(-0.5f, 0.5f, 0.5f, 0.f, 0.f),
        VERTEX(0.5f, 0.5f, 0.5f, 1.f, 0.f),
        VERTEX(0.5f, 0.5f, -0.5f, 1.f, 1.f),
        VERTEX(-0.5f, 0.5f, 0.5f, 0.f, 0.f),
        VERTEX(0.5f, 0.5f, -0.5f, 1.f, 1.f),
        VERTEX(-0.5f, 0.5f, -0.5f, 0.f, 1.f),

        // Bottom face

        VERTEX(-0.5f, -0.5f, -0.5f, 0.f, 0.f),
        VERTEX(0.5f, -0.5f, -0.5f, 1.f, 0.f),
        VERTEX(0.5f, -0.5f, 0.5f, 1.f, 1.f),
        VERTEX(-0.5f, -0.5f, -0.5f, 0.f, 0.f),
        VERTEX(0.5f, -0.5f, 0.5f, 1.f, 1.f),
        VERTEX(-0.5f, -0.5f, 0.5f, 0.f, 1.f)
};

constexpr int CUBE_VERTEX_COUNT = 36;

ChunkMesh::ChunkMesh(const Texture &arrayTexture) {
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    if (arrayTexture.getType() != TextureType::Tex2dArray)
        throw std::invalid_argument("texture must be array texture");

    arrayTextureId = arrayTexture.getGlHandle();

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0); // aPosition
    glEnableVertexAttribArray(1); // aTexCoord

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, position)));

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, texCoord)));

    glBindVertexArray(0);
}

void ChunkMesh::update(const BlockCache &blockCache, const Chunk &chunk, float sideLen, glm::vec3 offset) {
    std::vector<Vertex> vertices(chunk.getSideLen() * chunk.getSideLen() * chunk.getSideLen() * CUBE_VERTEX_COUNT);

    for (int x = 0; x < chunk.getSideLen(); ++x) {
        for (int y = 0; y < chunk.getSideLen(); ++y) {
            for (int z = 0; z < chunk.getSideLen(); ++z) {
                glm::vec3 origin = offset + sideLen * glm::vec3(x, y, z);

                int first = x * chunk.getSideLen() * chunk.getSideLen() + y * chunk.getSideLen() + z;
                first *= CUBE_VERTEX_COUNT;

                for (int face = 0; face < 6; ++face) {

                }

                auto &v = CUBE_VERTICES;

                memcpy(&vertices[first], CUBE_VERTICES, sizeof(Vertex) * CUBE_VERTEX_COUNT);

                for (int i = 0; i < CUBE_VERTEX_COUNT; ++i)
                    vertices[first + i].position = sideLen * vertices[first + i].position + origin;
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Vertex) * vertices.size()), vertices.data(),
                 GL_DYNAMIC_DRAW);

    vertexCount = static_cast<int>(vertices.size());
}

void ChunkMesh::draw() const {
    glBindTexture(GL_TEXTURE_2D_ARRAY, arrayTextureId);
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    glBindVertexArray(0);
}