//
// Created by egorv on 7/11/2023.
//

#include "utils/Utils.h"
#include "SkyboxMesh.h"

struct SkyboxVertex {
    glm::vec3 position;
};

constexpr SkyboxVertex VERTICES[8] = {
        {glm::vec3(-.5, -.5, .5)},
        {glm::vec3(.5, -.5, .5)},
        {glm::vec3(.5, .5, .5)},
        {glm::vec3(-.5, .5, .5)},
        {glm::vec3(-.5, -.5, -.5)},
        {glm::vec3(.5, -.5, -.5)},
        {glm::vec3(.5, .5, -.5)},
        {glm::vec3(-.5, .5, -.5)}
};

constexpr GLubyte INDICES[30] = {
        0, 1, 2, 0, 2, 3, // front
        5, 4, 7, 5, 7, 6, // back
        1, 5, 6, 1, 6, 2, // right
        4, 0, 3, 4, 3, 7, // left,
        3, 2, 6, 3, 6, 7, // top
};

SkyboxMesh::SkyboxMesh() {
    VAO = GlVertexArray::create();
    VAO.bind();

    VBO = GlVertexBuffer::create();
    VBO.bind();

    EBO = GlElementBuffer::create();
    EBO.bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkyboxVertex), BUF_PTR(SkyboxVertex, position));

    GlVertexArray::unbind();
    GlVertexBuffer::unbind();
}

void SkyboxMesh::draw() const {
    VAO.bind();
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_BYTE, nullptr);
    GlVertexArray::unbind();
}