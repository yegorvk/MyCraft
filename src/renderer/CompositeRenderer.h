//
// Created by egorv on 7/5/2023.
//

#ifndef SHITCRAFT_COMPOSITERENDERER_H
#define SHITCRAFT_COMPOSITERENDERER_H

#include "glm/glm.hpp"

#include "gl/GlResource.h"
#include "ViewFrustrum.h"

struct RenderState {
    ViewFrustrum frustrum;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::dvec3 playerPosition;
};

class CompositeRenderer {
public:
    CompositeRenderer() = default;

    explicit CompositeRenderer(glm::ivec2 viewportSize);

    void draw(const RenderState &state);

private:
    void createGBuffer();

    void createOutputBuffer();

    GlTexture<GL_TEXTURE_2D> gPosition, gNormal, gAlbedoSpec, oColorTex, oDepthStencilTex;
    GlFramebuffer gBuffer, output;
    glm::ivec2 viewportSize{};
};


#endif //SHITCRAFT_COMPOSITERENDERER_H
