//
// Created by egorv on 7/11/2023.
//

#include "utils/Utils.h"
#include "SimpleMesh.h"

SimpleMesh::SimpleMesh(int vertexCount, const SimpleVertex *vertices) : vertexCount(vertexCount) {
    VBO = GlVertexBuffer::create();
    VBO.bind();

    VAO = GlVertexArray::create();
    VAO.bind();

    glBufferData(GL_ARRAY_BUFFER, int(sizeof(SimpleVertex)) * vertexCount, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), BUF_PTR(SimpleVertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), BUF_PTR(SimpleVertex, texCoords));

    GlVertexArray::unbind();
    GlVertexBuffer::unbind();
}

void SimpleMesh::draw() const {
    VAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    GlVertexArray::unbind();
}