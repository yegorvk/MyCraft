//
// Created by egorv on 7/12/2023.
//

#include "GameContext.h"
#include "SkyRenderer.h"

SkyRenderer::SkyRenderer() : skyboxShader(&GameContext::global().getAssets().getShader("@shader/skybox")) {
    const char *skyBoxImages[6] = {
            "@image/skybox_right",
            "@image/skybox_left",
            "@image/skybox_top",
            "@image/skybox_bottom",
            "@image/skybox_front",
            "@image/skybox_back"
    };

    auto cubeMapBuilder = TextureFactory::textureCubeMap(TexSamplerOptions::linearClamp());
    auto &assets = GameContext::global().getAssets();

    for (int i = 0; i < 6; ++i)
        cubeMapBuilder.setFaceImage(i, assets.getImage(skyBoxImages[i]));

    texture = cubeMapBuilder.build();
}

void SkyRenderer::draw(const glm::mat4 &view, const Perspective &perspective, float skyDistance) const {
    glDepthMask(GL_FALSE);
    glCullFace(GL_FRONT);

    glm::mat4 projMat = glm::perspective(perspective.vFovRad * 1.5, perspective.aspectRatio, perspective.near,
                                         perspective.far);

    auto mvp = glm::mat4(glm::mat3(view));
    mvp = projMat * mvp;
    mvp = mvp * glm::scale(glm::mat4(1.f), glm::vec3(2.f * skyDistance));
    mvp = mvp * glm::translate(glm::mat4(1.f), glm::vec3(0, -0.2, 0));

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    skyboxShader->bind();
    skyboxShader->setMat4("mvp", mvp);

    mesh.draw();

    Shader::unbind();
    TextureCubeMap::unbind();

    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
}