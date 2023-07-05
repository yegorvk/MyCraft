//
// Created by egorv on 6/29/2023.
//

#ifndef SHITCRAFT_HUDRENDERER_H
#define SHITCRAFT_HUDRENDERER_H

#include "types.h"
#include "gl/Shader.h"
#include "gl/Texture.h"
#include "gl/GlResource.h"

class HUDRenderer {
public:
    HUDRenderer();

    void draw(float viewportAspectRatio) const;

private:
    GlVertexBuffer cursorVBO;
    GlVertexArray  cursorVAO;
    Texture cursorTexture;
    const Shader *cursorShader = nullptr;
};


#endif //SHITCRAFT_HUDRENDERER_H
