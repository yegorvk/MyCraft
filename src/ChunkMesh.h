//
// Created by egorv on 4/14/2023.
//

#ifndef SHITCRAFT_CHUNKMESH_H
#define SHITCRAFT_CHUNKMESH_H

#include "core/Drawable.h"
#include "types.h"
#include "glad/gl.h"
#include "renderer/Texture.h"

class ChunkMesh : public Drawable {
public:
    //inline ChunkMesh(uint vbo, uint vao, const Texture2D &texture) : vbo(vbo), vao(vao), texture(texture) {}

    inline ~ChunkMesh() {
        if (vao != 0) glDeleteVertexArrays(1, &vao);
        if (vbo != 0) glDeleteBuffers(1, &vbo);
    }

    inline void draw(const Transform &transform) const final {
        glBindVertexArray(vao);
        //TODO
    }
private:
    uint vbo = 0, vao = 0;
    //const Texture2D &texture;
};

class ChunkMeshBuilder {
private:

};

#endif //SHITCRAFT_CHUNKMESH_H
