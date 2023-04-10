//
// Created by egorv on 4/8/2023.
//

#include "SimpleCube.h"
#include "res.h"

static float vertices[] = {
        // position (xyz); vertex color (rgb); tex coords (uv)

        // front face (ccw order)
        -0.5f, -0.5f, -0.5f, 0.5f, 0.2f, 0.3f, 0.f, 0.f, // bottom left
        0.5f, -0.5f, -0.5f, 0.9f, 0.4f, 0.7f, 1.f, 0.f, // bottom right
        0.5f, 0.5f, -0.5f, 0.3f, 0.8f, 0.1f, 1.f, 1.f, // top right
        -0.5f, 0.5f, -0.5f, 0.1f, 0.2f, 0.3f, 1.f, 1.f, // top left

        // back face (cw order)
        -0.5f, -0.5f, 0.5f, 0.5f, 0.2f, 0.3f, 0.f, 0.f, // bottom left
        -0.5f, 0.5f, 0.5f, 0.1f, 0.2f, 0.3f, 1.f, 1.f, // top left
        0.5f, 0.5f, 0.5f, 0.3f, 0.8f, 0.1f, 1.f, 1.f, // top right
        0.5f, -0.5f, 0.5f, 0.9f, 0.4f, 0.7f, 1.f, 0.f, // bottom right
};

static uint indices[] = {
        // front face

        0, 1, 2,
        2, 3, 0,

        // back face

        4, 5, 6,
        4, 6, 7,

        // left face

        0, 3, 5,
        0, 5, 4,

        // right face

        2, 1, 7,
            2, 7, 6,

        // top face

        2, 6, 5,
        2, 5, 3,

        // bottom face

        0, 4, 7,
        0, 7, 1
};

SimpleCube::SimpleCube() :
        shader(Shader::compile(res::loadString("glsl/basic.vert").c_str(),
                               res::loadString("glsl/basic.frag").c_str())) {
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    shader.bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + 3 + 2), reinterpret_cast<void *>(0));

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + 3 + 2),
                          reinterpret_cast<void *>(sizeof(float) * 3));

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + 3 + 2),
                          reinterpret_cast<void *>(sizeof(float) * (3 + 3)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    Shader::unbind();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SimpleCube::draw(const Transform &transform) const {
    glBindVertexArray(vao);
    shader.bind();

    shader.setMat4("mvp", transform.transform);

    glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(uint), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    Shader::unbind();
}

SimpleCube::~SimpleCube() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vao);
}
