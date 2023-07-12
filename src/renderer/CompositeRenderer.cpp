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
    const auto viewProjection = state.perspective.computeProjectionMatrix() * state.viewMatrix;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ChunkRenderState chunkRendererState{state.playerPosition, viewProjection, state.frustrum};

    chunkRenderer.draw(chunkRendererState);
    skyRenderer.draw(state.viewMatrix, state.perspective, 1000.f);
    hudRenderer.draw(static_cast<float>(viewportSize.x) / static_cast<float>(viewportSize.y));
}