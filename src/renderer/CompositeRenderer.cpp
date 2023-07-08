//
// Created by egorv on 7/5/2023.
//

#include "gl/Texture.h"
#include "CompositeRenderer.h"

CompositeRenderer::CompositeRenderer(glm::ivec2 viewportSize) : viewportSize(viewportSize) {
    createGBuffer();
    createOutputBuffer();
}

void CompositeRenderer::draw(const RenderState &state) {
    if (gBuffer.getId() == 0)
        return;

    // gBuffer pass

    gBuffer.bind();

    // TODO fill gBuffers

    // Deferred pass

    output.bind();

    // TODO combine all gBuffers into one output buffer

    glClear(GL_COLOR_BUFFER_BIT);

    // TODO post-processing

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT);

    glBlitFramebuffer(0, 0, viewportSize.x, viewportSize.y, 0, 0, viewportSize.x, viewportSize.y, GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
}

void CompositeRenderer::createGBuffer() {
    gBuffer = GlFramebuffer::create();
    gBuffer.bind();

    // position

    gPosition = Texture2d::create();
    gPosition.bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, viewportSize.x, viewportSize.y, 0, GL_RGBA, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition.getId(), 0);

    // normal

    gNormal = Texture2d::create();
    gNormal.bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, viewportSize.x, viewportSize.y, 0, GL_RGBA, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal.getId(), 0);

    // Albedo (color) + specular

    gAlbedoSpec = Texture2d::create();
    gAlbedoSpec.bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, viewportSize.x, viewportSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec.getId(), 0);

    gl_enum_type attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    // Depth buffer

    const auto depthStencilTex = Texture2d::create();
    depthStencilTex.bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, viewportSize.x, viewportSize.y, 0, GL_DEPTH_STENCIL,
                 GL_UNSIGNED_INT_24_8, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilTex.getId(), 0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    GlFramebuffer::unbind();
}

void CompositeRenderer::createOutputBuffer() {
    output = GlFramebuffer::create();
    output.bind();

    oColorTex = Texture2d::create();
    oColorTex.bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, viewportSize.x, viewportSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, oColorTex.getId(), 0);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    oDepthStencilTex = Texture2d::create();
    oDepthStencilTex.bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, viewportSize.x, viewportSize.y, 0, GL_DEPTH_STENCIL,
                 GL_UNSIGNED_INT_24_8, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, oDepthStencilTex.getId(), 0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    GlFramebuffer::unbind();
}