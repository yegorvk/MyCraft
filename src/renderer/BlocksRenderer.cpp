//
// Created by egorv on 4/26/2023.
//

#include "ViewFrustrum.h"

#include "Context.h"
#include "BlocksRenderer.h"

BlocksRenderer::BlocksRenderer()
: shader(Context::global().getAssets().getShader("@shader/chunk")), texture(createArrayTexture()) {
    chunk = ChunkMesh(texture);
    chunk.update(BlockCache(), Chunk(4), 0.2f);
}

void BlocksRenderer::draw(const Transform &transform) const {
    shader.bind();
    shader.setMat4("mvp", transform.mvp);

    BoundingBox box(glm::vec3(0.f), glm::vec3(16.f * 0.2f));

    if (box.isOnFrustrum(transform.frustrum))
        chunk.draw();
}

Texture BlocksRenderer::createArrayTexture() {
    const auto &image = Context::global().getAssets().getImage("@img/grass_top");
    std::array<std::reference_wrapper<const Image>, 1> images = {{std::cref(image)}};

    return Texture::texture2dArray(image.getWidth(), image.getHeight(), image.getFormat(), images.begin(), images.end());
}
