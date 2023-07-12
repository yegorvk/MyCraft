//
// Created by egorv on 7/11/2023.
//

#ifndef SHITCRAFT_SIMPLEMESH_H
#define SHITCRAFT_SIMPLEMESH_H

#include "glm/glm.hpp"

#include "gl/GlResource.h"

struct SimpleVertex {
    glm::vec3 position;
    glm::vec2 texCoords;
};

class SimpleMesh {
public:
    SimpleMesh(int vertexCount, const SimpleVertex *vertices);

    void draw() const;
private:
    GlVertexBuffer VBO;
    GlVertexArray VAO;
    int vertexCount;
};


#endif //SHITCRAFT_SIMPLEMESH_H
