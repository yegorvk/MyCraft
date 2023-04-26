//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_SIMPLECUBE_H
#define SHITCRAFT_SIMPLECUBE_H

#include "core/Drawable.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

class SimpleCube : public Drawable {
public:
    SimpleCube();

    ~SimpleCube();

    void draw(const Transform &transform) const final;
private:
    Texture texture;
    const Shader &shader;
    uint vbo = 0, vao = 0;
};

#endif //SHITCRAFT_SIMPLECUBE_H
