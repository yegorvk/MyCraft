//
// Created by egorv on 7/6/2023.
//

#ifndef SHITCRAFT_RECTMESH_H
#define SHITCRAFT_RECTMESH_H

#include "glm/glm.hpp"

#include "gl/GlResource.h"

class RectMesh {
public:
    explicit RectMesh(glm::vec2 size = {1.f, 1.f}, glm::vec3 bottomLeft = {-.5f, -.5f, 0.f});

    void draw() const;

private:
    GlVertexBuffer VBO;
    GlElementBuffer EBO;
    GlVertexArray VAO;
};


#endif //SHITCRAFT_RECTMESH_H
