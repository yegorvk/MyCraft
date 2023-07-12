//
// Created by egorv on 6/29/2023.
//

#include "HUDRenderer.h"
#include "GameContext.h"
#include "gl/GlResource.h"

constexpr TexSamplerOptions TEX_OPTIONS = TexSamplerOptions::nearestClamp();

HUDRenderer::HUDRenderer()
        : cursorTexture(
        TextureFactory::texture2d(GameContext::global().getAssets().getImage("@image/cursor"), TEX_OPTIONS)),
          cursorShader(&GameContext::global().getAssets().getShader("@shader/rect")) {}

void HUDRenderer::draw(float viewportAspectRatio) const {
    glDisable(GL_DEPTH_TEST);

    cursorTexture.bind();
    cursorShader->bind();

    glm::mat4 mvp(1.f);

    if (viewportAspectRatio > 1.f)
        mvp = glm::scale(mvp, glm::vec3(1.f / viewportAspectRatio, 1.f, 1.f));
    else
        mvp = glm::scale(mvp, glm::vec3(1.f, viewportAspectRatio, 1.f));

    mvp = glm::scale(mvp, glm::vec3(0.1f));

    cursorShader->setMat4("mvp", mvp);

    mesh.draw();

    glUseProgram(0);
    Texture2d::unbind();

    glEnable(GL_DEPTH_TEST);
}