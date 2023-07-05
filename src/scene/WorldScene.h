//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_WORLDSCENE_H
#define SHITCRAFT_WORLDSCENE_H

#include "core/Node.h"
#include "world/World.h"
#include "camera/FreeCamera.h"
#include "renderer/WorldRenderer.h"
#include "renderer/HUDRenderer.h"
#include "registry/BlockRegistry.h"

class WorldScene : public Node {
public:
    WorldScene();

    void onPreDraw() const override;

    void onUpdate(uint64_t deltaMs) override;

    bool onHandleEvent(const SDL_Event &event) override;

    ~WorldScene() override = default;

private:
    FreeCamera camera;

    glm::vec2 viewportSize{};

    double winAspectRatio = 1.0;
    glm::mat4 projMat{1.f};

    World world;

    WorldRenderer blocksRenderer;
    HUDRenderer hudRenderer;

    BlockRegistry blockReg;
};


#endif //SHITCRAFT_WORLDSCENE_H
