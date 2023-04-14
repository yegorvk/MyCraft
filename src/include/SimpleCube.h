//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_SIMPLECUBE_H
#define SHITCRAFT_SIMPLECUBE_H

#include "Drawable.h"
#include "Shader.h"

class SimpleCube : public Drawable {
public:
    SimpleCube();

    ~SimpleCube();

    void draw(const Transform &transform) const final;

private:
    Shader shader;

    uint vbo = 0, vao = 0;
};

#endif //SHITCRAFT_SIMPLECUBE_H
