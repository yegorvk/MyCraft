//
// Created by egorv on 7/11/2023.
//

#ifndef SHITCRAFT_SKYBOXMESH_H
#define SHITCRAFT_SKYBOXMESH_H

#include "gl/GlResource.h"
#include "SimpleMesh.h"

class SkyboxMesh {
public:
    SkyboxMesh();

    void draw() const;

private:
    GlVertexArray VAO;
    GlVertexBuffer VBO;
    GlElementBuffer EBO;
};


#endif //SHITCRAFT_SKYBOXMESH_H
