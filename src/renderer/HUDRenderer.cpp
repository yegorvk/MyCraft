//
// Created by egorv on 6/29/2023.
//

#include "HUDRenderer.h"
#include "GameContext.h"
#include "gl/GlResource.h"

struct PackedChunkVertex {
    glm::vec2 position;
    glm::vec2 texCoords;
};

constexpr PackedChunkVertex CURSOR_VERTICES[] = {
        {glm::vec2(-0.5f, -0.5f), glm::vec2(0, 0)},
        {glm::vec2(0.5f, -0.5f),  glm::vec2(1, 0)},
        {glm::vec2(-0.5f, 0.5f),  glm::vec2(0, 1)},
        {glm::vec2(0.5f, -0.5f),  glm::vec2(1, 0)},
        {glm::vec2(0.5f, 0.5f),   glm::vec2(1, 1)},
        {glm::vec2(-0.5f, 0.5f),  glm::vec2(0, 1)}
};

HUDRenderer::HUDRenderer()
        : cursorTexture(Texture::texture2d(GameContext::global().getAssets().getImage("@image/cursor"))),
          cursorShader(&GameContext::global().getAssets().getShader("@shader/cursor")) {
    cursorVBO = GlVertexBuffer::create();
    cursorVAO = GlVertexArray::create();

    cursorVBO.bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(CURSOR_VERTICES), CURSOR_VERTICES, GL_STATIC_DRAW);

    cursorVAO.bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PackedChunkVertex),
                          reinterpret_cast<void *>(offsetof(PackedChunkVertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PackedChunkVertex),
                          reinterpret_cast<void *>(offsetof(PackedChunkVertex, texCoords)));

    GlVertexArray::unbind();
}

void HUDRenderer::draw(float viewportAspectRatio) const {
    cursorTexture.bind();
    cursorShader->bind();

    glm::mat4 mvp(1.f);

    if (viewportAspectRatio > 1.f)
        mvp = glm::scale(mvp, glm::vec3(1.f / viewportAspectRatio, 1.f, 1.f));
    else
        mvp = glm::scale(mvp, glm::vec3(1.f, viewportAspectRatio, 1.f));

    mvp = glm::scale(mvp, glm::vec3(0.1f));

    cursorShader->setMat4("mvp", mvp);

    cursorVBO.bind();
    cursorVAO.bind();

    glDrawArrays(GL_TRIANGLES, 0, sizeof(CURSOR_VERTICES) / sizeof(PackedChunkVertex));
    GlVertexArray::unbind();

    glUseProgram(0);
}