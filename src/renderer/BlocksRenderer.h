//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_BLOCKSRENDERER_H
#define SHITCRAFT_BLOCKSRENDERER_H

#include "core/Drawable.h"
#include "ChunkMesh.h"

class BlocksRenderer : public Drawable {
public:
    BlocksRenderer();

    void draw(const Transform &transform) const final;

private:
    static Texture createArrayTexture();

    const Shader &shader;
    ChunkMesh chunk;
    Texture texture;
};


#endif //SHITCRAFT_BLOCKSRENDERER_H
