//
// Created by egorv on 4/8/2023.
//

#include "SimpleCube.h"
#include "Context.h"

#include "renderer/Vertex.h"

#define VERTEX(x, y, z, u, v) Vertex(glm::vec3(x, y, z), glm::vec2(u, v))

static Vertex vertices[] = {
        // Front face

        VERTEX(-0.5f, -0.5f, 0.5f, 0.f, 0.f), // bottom left
        VERTEX(0.5f, -0.5f, 0.5f, 1.f, 0.f), // bottom right
        VERTEX(0.5f, 0.5f, 0.5f, 1.f, 1.f), // top right
        VERTEX(-0.5f, -0.5f, 0.5f, 0.f, 0.f), // bottom left
        VERTEX(0.5f, 0.5f, 0.5f, 1.f, 1.f), // top right
        VERTEX(-0.5f, 0.5f, 0.5f, 0.f, 1.f), // top left

        // Back face

        VERTEX(-0.5f, -0.5f, -0.5f, 1.f, 0.f), // bottom left
        VERTEX(-0.5f, 0.5f, -0.5f, 1.f, 1.f), // top left
        VERTEX(0.5f, 0.5f, -0.5f, 0.f, 1.f), // top right
        VERTEX(-0.5f, -0.5f, -0.5f, 1.f, 0.f), // bottom left
        VERTEX(0.5f, 0.5f, -0.5f, 0.f, 1.f), // top right
        VERTEX(0.5f, -0.5f, -0.5f, 0.f, 0.f), // bottom right

        // Left face

        VERTEX(-0.5f, -0.5f, 0.5f, 1.f, 0.f),
        VERTEX(-0.5f, 0.5f, 0.5f, 1.f, 1.f),
        VERTEX(-0.5f, 0.5f, -0.5f, 0.f, 1.f),
        VERTEX(-0.5f, -0.5f, 0.5f, 1.f, 0.f),
        VERTEX(-0.5f, 0.5f, -0.5f, 0.f, 1.f),
        VERTEX(-0.5f, -0.5f, -0.5f, 0.f, 0.f),

        // Right face

        VERTEX(0.5f, -0.5f, 0.5f, 0.f, 0.f),
        VERTEX(0.5f, -0.5f, -0.5f, 1.f, 0.f),
        VERTEX(0.5f, 0.5f, -0.5f, 1.f, 1.f),
        VERTEX(0.5f, -0.5f, 0.5f, 0.f, 0.f),
        VERTEX(0.5f, 0.5f, -0.5f, 1.f, 1.f),
        VERTEX(0.5f, 0.5f, 0.5f, 0.f, 1.f),

        // Top face

        VERTEX(-0.5f, 0.5f, 0.5f, 0.f, 0.f),
        VERTEX(0.5f, 0.5f, 0.5f, 1.f, 0.f),
        VERTEX(0.5f, 0.5f, -0.5f, 1.f, 1.f),
        VERTEX(-0.5f, 0.5f, 0.5f, 0.f, 0.f),
        VERTEX(0.5f, 0.5f, -0.5f, 1.f, 1.f),
        VERTEX(-0.5f, 0.5f, -0.5f, 0.f, 1.f),

        // Bottom face

        VERTEX(-0.5f, -0.5f, -0.5f, 0.f, 0.f),
        VERTEX(0.5f, -0.5f, -0.5f, 1.f, 0.f),
        VERTEX(0.5f, -0.5f, 0.5f, 1.f, 1.f),
        VERTEX(-0.5f, -0.5f, -0.5f, 0.f, 0.f),
        VERTEX(0.5f, -0.5f, 0.5f, 1.f, 1.f),
        VERTEX(-0.5f, -0.5f, 0.5f, 0.f, 1.f)
};

SimpleCube::SimpleCube() :
        shader(Shader::compile(Context::global().getAssets().getStr("shaders/basic.vert").c_str(),
                               Context::global().getAssets().getStr("shaders/basic.frag").c_str())) {
    glGenBuffers(1, &vbo);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    shader.bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, texCoord)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    Shader::unbind();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SimpleCube::draw(const Transform &transform) const {
    const auto &tex = Context::global().getAssets().getTex2D("textures/box.png");

    glActiveTexture(GL_TEXTURE0);
    tex.bind();

    glBindVertexArray(vao);
    shader.bind();

    shader.setMat4("mvp", transform.transform);

    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(Vertex));

    glBindVertexArray(0);
    Shader::unbind();
}

SimpleCube::~SimpleCube() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
