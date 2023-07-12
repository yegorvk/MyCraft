//
// Created by egorv on 7/12/2023.
//

#ifndef SHITCRAFT_SKYRENDERER_H
#define SHITCRAFT_SKYRENDERER_H

#include "gl/Shader.h"
#include "Perspective.h"
#include "mesh/SkyboxMesh.h"

class SkyRenderer {
public:
    SkyRenderer();

    void draw(const glm::mat4 &view, const Perspective &perspective, float skyDistance) const;

private:
    SkyboxMesh mesh;
    TextureCubeMap texture;
    const Shader *skyboxShader;
};


#endif //SHITCRAFT_SKYRENDERER_H
