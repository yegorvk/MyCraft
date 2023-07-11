//
// Created by egorv on 7/5/2023.
//

#include "gl/Texture.h"

#include "GameContext.h"
#include "CompositeRenderer.h"

CompositeRenderer::CompositeRenderer() : viewportSize() {}

void CompositeRenderer::resize(glm::ivec2 newViewportSize) {
    if (newViewportSize == viewportSize)
        return;

    viewportSize = newViewportSize;
}

void CompositeRenderer::draw(const RenderState &state) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    const auto viewProjection = state.projectionMatrix * state.viewMatrix;
    ChunkRenderState chunkRendererState{state.playerPosition, viewProjection, state.frustrum};

    chunkRenderer.draw(chunkRendererState);

    glDisable(GL_DEPTH_TEST);

    hudRenderer.draw(static_cast<float>(viewportSize.x) / static_cast<float>(viewportSize.y));
}