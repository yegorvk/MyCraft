//
// Created by egorv on 7/6/2023.
//

#include "utils/Utils.h"
#include "SimpleVertex.h"
#include "RectMesh.h"

RectMesh::RectMesh(glm::vec2 size, glm::vec3 bottomLeft) {
    SimpleVertex vertices[4] = {
            {bottomLeft,                                glm::vec2(0, 0)},
            {bottomLeft + glm::vec3(size.x, 0, 0),      glm::vec2(1, 0)},
            {bottomLeft + glm::vec3(size.x, size.y, 0), glm::vec2(1, 1)},
            {bottomLeft + glm::vec3(0, size.y, 0),      glm::vec2(0, 1)},
    };

    unsigned int indices[6] = {
            0, 1, 2,
            0, 2, 3
    };

    VAO = GlVertexArray::create();
    VAO.bind();

    VBO = GlVertexBuffer::create();
    VBO.bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    EBO = GlElementBuffer::create();
    EBO.bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), BUF_PTR(SimpleVertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), BUF_PTR(SimpleVertex, texCoords));

    GlVertexArray::unbind();
}

void RectMesh::draw() const {
    VAO.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    GlVertexArray::unbind();
}